#include "FS.h"
#include <cstring>
#include <cstdlib>
#include <strings.h>
#include "drivers/SpiNorFlash.h"
#include <littlefs/lfs.h>
#include <lvgl/lvgl.h>

using namespace Pinetime::Controllers;


/*
 * External Flash MAP (4 MBytes)
 *
 * 0x000000 +---------------------------------------+
 *          |  Bootloader Assets                    |
 *          |  256 KBytes                           |
 *          |                                       |
 * 0x040000 +---------------------------------------+
 *          |  OTA                                  |
 *          |  464 KBytes                           |
 *          |                                       |
 *          |                                       |
 *          |                                       |
 * 0x0B4000 +---------------------------------------+
 *          |  File System                          |
 *          |                                       |
 *          |                                       |
 *          |                                       |
 *          |                                       |
 * 0x3F6000 +---------------------------------------+
 *          |  System Settings                      |
 *          |  40 KBytes                            |
 * 0x400000 +---------------------------------------+
 *
 */

//#define FS_HEADER   0x0B4000 // File Image Header
//#define FS_ENDFILES 0x3F6000 // File System END
// File system size = 0x340000 - 3 407 872 Bytes - 3.328 MBytes
// the last 40 Kbytes is reserved for System Settings


static constexpr size_t BLOCK_SIZE_BYTES = 4096;


/*
*   Interface between littlefs and SpiNorFlash
*/

static int read(const struct lfs_config *c, lfs_block_t block,
                lfs_off_t off, void *buffer, lfs_size_t size) {
    Pinetime::Controllers::FS& lfs = *(reinterpret_cast<Pinetime::Controllers::FS*>(c->context));
    const size_t address = lfs.mStartAddress + (block * BLOCK_SIZE_BYTES) + off;
    lfs.mDriver.Read(address, (uint8_t*)buffer, size);
    // TODO assumes READ was successful
    return 0u;
}

static int prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size) {
    Pinetime::Controllers::FS& lfs = *(reinterpret_cast<Pinetime::Controllers::FS*>(c->context));
    const size_t address = lfs.mStartAddress + (block * BLOCK_SIZE_BYTES) + off;
    lfs.mDriver.Write(address, (uint8_t*)buffer, size);
    return lfs.mDriver.ProgramFailed() ? -1u : 0u;
}

static int erase(const struct lfs_config *c, lfs_block_t block) {
    Pinetime::Controllers::FS& lfs = *(reinterpret_cast<Pinetime::Controllers::FS*>(c->context));
    const size_t address = lfs.mStartAddress + (block * BLOCK_SIZE_BYTES);
    lfs.mDriver.SectorErase(address);
    return lfs.mDriver.EraseFailed() ? -1u : 0u;
}

static int sync(const struct lfs_config *c) {
    // no hardware caching used
    return 0u;
}


const static struct lfs_config baseLfsConfig = {
    .read = read,
    .prog = prog,
    .erase = erase,
    .sync = sync,

    .read_size = 16,
    .prog_size = 8,
    .block_size = BLOCK_SIZE_BYTES,
    .block_cycles = 1000u,

    .cache_size = 16,
    .lookahead_size = 16,

    .name_max = 50,
    .attr_max = 50

};

constexpr struct lfs_config createLfsConfig(Pinetime::Controllers::FS& fs, const size_t totalSizeBytes) {
    struct lfs_config config = baseLfsConfig;
    config.context = &fs;
    config.block_count = totalSizeBytes / BLOCK_SIZE_BYTES;
    return config;
}

FS::FS(Pinetime::Drivers::SpiNorFlash& driver,
       const size_t startAddress,
       const size_t sizeBytes) :
    mDriver{driver},
    mStartAddress{startAddress},
    mSize{sizeBytes},
    mLfsConfig{createLfsConfig(*this, sizeBytes)} { }


void FS::Init() {

    lfs_file_t file;    

    // try mount
    //lfs_format(&mLfs, &mLfsConfig);
    int err = lfs_mount(&mLfs, &mLfsConfig);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&mLfs, &mLfsConfig);
        err = lfs_mount(&mLfs, &mLfsConfig);
        if (err) {
            return;
        }
    }
    VerifyResource();
    LVGLFileSystemInit();

    lfs_file_open(&mLfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&mLfs, &file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    lfs_file_rewind(&mLfs, &file);
    lfs_file_write(&mLfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&mLfs, &file);


}

void FS::VerifyResource() {
    // validate the resource metadata
    fsValid = true;
}

void FS::FileOpen(lfs_file_t* file_p, const char* fileName, const int flags) {
    if ( !fsValid ) return;
    lfs_file_open(&mLfs, file_p, fileName, flags);
}

void FS::FileClose(lfs_file_t* file_p) {
    if ( !fsValid ) return;
    lfs_file_close(&mLfs, file_p);
}

void FS::Delete(const char* fileName) {
    if ( !fsValid ) return;
    lfs_remove(&mLfs, fileName);
}

void FS::FileRead(lfs_file_t* file_p, uint8_t* buff, uint32_t size) {
    if ( !fsValid ) return;
    lfs_file_read(&mLfs, file_p, buff, size);
}

void FS::FileWrite(lfs_file_t* file_p, const uint8_t* buff, uint32_t size) {
    if ( !fsValid ) return;
    lfs_file_write(&mLfs, file_p, buff, size);
}

void FS::FileSeek(lfs_file_t* file_p, uint32_t pos) {
    if ( !fsValid ) return;
    lfs_file_seek(&mLfs, file_p, pos, LFS_SEEK_SET);
}

void FS::MkDir(const char* path) {
    if ( !fsValid ) return;
    lfs_mkdir(&mLfs, path);
}


/*

    ----------- LVGL filesystem integration -----------

*/

lv_fs_res_t lvglOpen(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode) {

    lfs_file_t* file = static_cast<lfs_file_t *>(file_p);
    FS* filesys = static_cast<FS *>(drv->user_data);
    filesys->FileOpen(file, path, LFS_O_RDONLY);

    if (file->type == 0) {
        return LV_FS_RES_FS_ERR;
    } else {
        return LV_FS_RES_OK;
    }
}

lv_fs_res_t lvglClose(lv_fs_drv_t* drv, void* file_p) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    lfs_file_t* file = static_cast<lfs_file_t *>(file_p);
    filesys->FileClose( file );

    return LV_FS_RES_OK;
}

lv_fs_res_t lvglRead(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    lfs_file_t* file = static_cast<lfs_file_t *>(file_p);
    filesys->FileRead(file, (uint8_t *)buf, btr);
    *br = btr;
    return LV_FS_RES_OK;
}

lv_fs_res_t lvglSeek(lv_fs_drv_t* drv, void* file_p, uint32_t pos) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    lfs_file_t* file = static_cast<lfs_file_t *>(file_p);
    filesys->FileSeek(file, pos);
    return LV_FS_RES_OK;
}


void FS::LVGLFileSystemInit() {

    if ( !fsValid ) return;
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    fs_drv.file_size = sizeof(lfs_file_t);
    fs_drv.letter = 'F';
    fs_drv.open_cb = lvglOpen;
    fs_drv.close_cb = lvglClose;
    fs_drv.read_cb = lvglRead;
    fs_drv.seek_cb = lvglSeek;

    fs_drv.user_data = this;

    lv_fs_drv_register(&fs_drv);

}
#include "FS.h"
#include <cstring>
#include <cstdlib>
#include <strings.h>

using namespace Pinetime::Controllers;


const uint8_t rawData[1] = {
    0x00
};

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
 *          |  File System FAT                      |
 *          |  4 KBytes                             |
 * 0x0B5000 +---------------------------------------+
 *          |  File System FILES                    |
 *          |  3.328 MBytes                         |
 *          |                                       |
 *          |                                       |
 *          |                                       |
 *          |                                       |
 * 0x3F6000 +---------------------------------------+
 *          |  System Settings                      |
 *          |  40 KBytes                            |
 * 0x400000 +---------------------------------------+
 */

#define FS_FAT      0x0B4000 // File System FAT
#define FS_ENDFAT   0x0B5000

#define FS_FILES    0x0B6000 // File System FILES
#define FS_ENDFILES 0x3F6000 // File System END
// File system size = 0x340000 - 3 407 872 Bytes - 3.328 MBytes
// the last 40 Kbytes is reserved for System Settings

FS::FS(Pinetime::Drivers::SpiNorFlash &spiNorFlash) : spiNorFlash{spiNorFlash} {}

void FS::FileDemo() {
    file_t file;

    // write fat
    spiNorFlash.SectorErase(FS_FAT);

    uint8_t filePath[] = "demo1.bin\0";

    std::memcpy(file.name, filePath, sizeof(filePath));
    file.offset = 0x00;
    file.size = 0xF2;
    file.end = 0x00;

    uint8_t writeBuffer[41];
    std::memcpy(writeBuffer, &file, sizeof(file));

    spiNorFlash.Write( FS_FAT, writeBuffer, sizeof(file));

    // write file

    for (uint32_t erased = 0; erased < sizeof(rawData); erased += 0x1000) {
        spiNorFlash.SectorErase(FS_FILES + erased);

        static constexpr uint32_t memoryChunkSize = 200;
        uint8_t writeBuffer[memoryChunkSize];
        for(uint32_t offset = 0; offset < sizeof(rawData); offset+=memoryChunkSize) {
            std::memcpy(writeBuffer, &rawData[offset], memoryChunkSize);
            spiNorFlash.Write(FS_FILES + offset, writeBuffer, memoryChunkSize);
        }
    }
}

void FS::FileOpen(uint8_t *fileName) {

    uint8_t buffer[41];

    for (uint32_t fatOffset = 0; fatOffset < FS_ENDFAT; fatOffset += sizeof(currFile)) {
        spiNorFlash.Read( FS_FAT + fatOffset, buffer, sizeof(currFile) );
        std::memcpy(&currFile, buffer, sizeof(currFile));
        
        if ( currFile.name[0] != 0xFF ) {
            // file found, is the file I need ?
            if ((strcasecmp((char*)fileName, (char*)currFile.name)) == 0) {
                // yes
                readAddrOffset = currFile.offset;
                fileReady = true;
                return;
            }
        } else {
            // no more files in FAT
            fileReady = false;
            return;
        }
    }
    fileReady = false;
    return;
}


void FS::FileClose() {

    fileReady = false;
    readAddrOffset = 0x00;

}


void FS::FileRead(uint8_t *buff, uint32_t size) {
  
    spiNorFlash.Read( FS_FILES + readAddrOffset, (uint8_t *)buff, size );
  
}

void FS::FileSeek(uint32_t pos) {
  
    readAddrOffset = currFile.offset + pos;
  
}

uint32_t FS::FileTell() {    

    return readAddrOffset - currFile.offset;
  
}


lv_fs_res_t FSOpen(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    filesys->FileOpen((uint8_t *)path);

    if (filesys->FileIsReady()) {
        return LV_FS_RES_OK;
    } else {
        return LV_FS_RES_FS_ERR;
    }
}

lv_fs_res_t FSClose(lv_fs_drv_t* drv, void* file_p) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    filesys->FileClose();

    return LV_FS_RES_OK;
}

lv_fs_res_t FSRead(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    filesys->FileRead((uint8_t *)buf, btr);
    *br = btr;
    return LV_FS_RES_OK;
}

lv_fs_res_t FSSeek(lv_fs_drv_t* drv, void* file_p, uint32_t pos) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    filesys->FileSeek(pos);
    return LV_FS_RES_OK;
}

lv_fs_res_t FSTell(struct _lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    *pos_p = filesys->FileTell();
    return LV_FS_RES_OK;
}


void FS::LVGLFileSystemInit() {
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    fs_drv.file_size = sizeof(file_t);
    fs_drv.letter = 'F';
    fs_drv.open_cb = FSOpen;
    fs_drv.close_cb = FSClose;
    fs_drv.read_cb = FSRead;
    fs_drv.seek_cb = FSSeek;
    fs_drv.tell_cb = FSTell;

    fs_drv.user_data = this; 

    lv_fs_drv_register(&fs_drv);

}
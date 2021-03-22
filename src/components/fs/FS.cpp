#include "FS.h"
#include <cstring>
#include <cstdlib>
#include <strings.h>

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
 *          |                                       |
 * 0x0B4010 +---------------------------------------+
 *          |  File System FAT                      |
 *          |  8 KBytes                             |
 * 0x0B6000 +---------------------------------------+
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

#define FS_HEADER   0x0B4000 // File Image Header
#define FS_FAT      0x0B4010 // File System FAT
#define FS_ENDFAT   0x0B5000

#define FS_FILES    0x0B6000 // File System FILES
#define FS_ENDFILES 0x3F6000 // File System END
// File system size = 0x340000 - 3 407 872 Bytes - 3.328 MBytes
// the last 40 Kbytes is reserved for System Settings

FS::FS(Pinetime::Drivers::SpiNorFlash &spiNorFlash) : spiNorFlash{spiNorFlash} {}

void FS::FileDemo() {
    
}

void FS::FileOpen(uint8_t *fileName, void* file_p) {

    uint8_t buffer[44];

    for (uint32_t fatOffset = 0; fatOffset < FS_ENDFAT; fatOffset += sizeof(currFile)) {
        spiNorFlash.Read( FS_FAT + fatOffset, buffer, sizeof(currFile) );
        std::memcpy(&currFile, buffer, sizeof(currFile));
        
        if ( currFile.name[0] != 0xFF ) {
            // file found, is the file I need ?
            if ((strcasecmp((char*)fileName, (char*)currFile.name)) == 0) {
                // yes
                readAddrOffset = 0;
                fileReady = true;
                //file_p = &currFile;
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

    // Fix for erro reading more than 240 bytes from flash ....
    //
    if ( size > 240 ) {
        uint32_t half = size / 2;
        spiNorFlash.Read( FS_FILES + currFile.offset + readAddrOffset, (uint8_t *)buff, half );
        spiNorFlash.Read( FS_FILES + currFile.offset + readAddrOffset + half, (uint8_t *)buff + half, half );
    }else {
        spiNorFlash.Read( FS_FILES + currFile.offset + readAddrOffset, (uint8_t *)buff, size );
    }
  
}

void FS::FileSeek(uint32_t pos) {
  
    readAddrOffset = pos;
  
}

/*
uint32_t FS::FileTell() {    

    return readAddrOffset;
  
}
*/

lv_fs_res_t FSOpen(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    filesys->FileOpen((uint8_t *)path, file_p);

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

/*
lv_fs_res_t FSTell(struct _lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p) {
    FS* filesys = static_cast<FS *>(drv->user_data);
    *pos_p = filesys->FileTell();
    return LV_FS_RES_OK;
}
*/

void FS::LVGLFileSystemInit() {
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    //fs_drv.file_size = sizeof(file_t);
    fs_drv.letter = 'F';
    fs_drv.open_cb = FSOpen;
    fs_drv.close_cb = FSClose;
    fs_drv.read_cb = FSRead;
    fs_drv.seek_cb = FSSeek;
    //fs_drv.tell_cb = FSTell;

    fs_drv.user_data = this; 

    lv_fs_drv_register(&fs_drv);

}
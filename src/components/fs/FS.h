#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "drivers/SpiNorFlash.h"

typedef struct {
    uint8_t name[32];
    uint32_t offset;
    uint32_t size;
    uint8_t end;
} file_t;

namespace Pinetime {
  namespace Controllers {
    class FS {
        public:
            FS(Pinetime::Drivers::SpiNorFlash& spiNorFlash);

            void LVGLFileSystemInit();

            void FileDemo();

            void FileOpen(uint8_t *fileName, void* file_p);
            void FileClose();
            void FileRead(uint8_t *buff, uint32_t size);
            void FileSeek(uint32_t pos);
            //uint32_t FileTell();

            bool FileIsReady() const { return fileReady; };
        private:
            Pinetime::Drivers::SpiNorFlash& spiNorFlash;

            uint32_t readAddrOffset = 0;
            bool fileReady = false;
            file_t currFile;

    };
  }
}

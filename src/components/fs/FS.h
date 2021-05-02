#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "drivers/SpiNorFlash.h"

typedef struct {
    uint8_t name[32];
    uint32_t offset;
    uint32_t size;
    uint8_t end;
} file_s;

typedef struct {
    uint32_t readAddrOffset = 0x00;
    uint32_t offset = 0x00;
    bool isOpen = false;
} file_t;

namespace Pinetime {
  namespace Controllers {
    class FS {
        public:
            FS(Pinetime::Drivers::SpiNorFlash& spiNorFlash);

            void LVGLFileSystemInit();

            void FileOpen(void* file_p, uint8_t *fileName);
            void FileClose(void* file_p);
            void FileRead(void* file_p, uint8_t *buff, uint32_t size);
            void FileSeek(void* file_p, uint32_t pos);
            void VerifyResource();

            void FormatFS();
        private:
            Pinetime::Drivers::SpiNorFlash& spiNorFlash;
            bool fsValid = false;

    };
  }
}

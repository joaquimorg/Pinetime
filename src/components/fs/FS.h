#pragma once

#include <cstdint>
#include "drivers/SpiNorFlash.h"
#include <littlefs/lfs.h>


namespace Pinetime {
  namespace Controllers {
    class FS {
        public:
            FS(Pinetime::Drivers::SpiNorFlash&,
                const size_t startAddress,
                const size_t sizeBytes);

            Pinetime::Drivers::SpiNorFlash& mDriver;
            const size_t mStartAddress;
            const size_t mSize;

            lfs_t mLfs;

            void Init();
            void LVGLFileSystemInit();

            void FileOpen(lfs_file_t* file_p, const char* fileName, const int flags);
            void FileClose(lfs_file_t* file_p);
            void Delete(const char* fileName);
            void FileRead(lfs_file_t* file_p, uint8_t* buff, uint32_t size);
            void FileWrite(lfs_file_t* file_p, const uint8_t* buff, uint32_t size);
            void FileSeek(lfs_file_t* file_p, uint32_t pos);

            void MkDir(const char* path);

            void VerifyResource();

        private:
            bool fsValid = false;
            
            const struct lfs_config mLfsConfig;

    };
  }
}

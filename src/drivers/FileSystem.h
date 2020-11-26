#pragma once
#include <cstddef>
#include <cstdint>

#include "littlefs/lfs.h"

namespace Pinetime {
  namespace Drivers {
    class Spi;
    class SpiNorFlash;
    class FileSystem {
        public:
            explicit FileSystem(SpiNorFlash& spiNorFlash);
            ~FileSystem();

            virtual uint32_t read();

            virtual int mount();
            // Format a file system. Results in an empty and mounted file system.
            //virtual int format();

            // Remove a file from the file system.
            virtual int remove(const char *path);

            // Rename a file in the file system.
            virtual int rename(const char *path, const char *newpath);

            // Store information about the file in a stat structure
            virtual int stat(const char *path, lfs_info *info);

            // Create a directory in the file system.
            virtual int mkdir(const char *path);
      
            // Open a file on the filesystem
            virtual int file_open(lfs_file_t *file, const char *path, int flags);

            // Close a file
            virtual int file_close(lfs_file_t file);

            // Read the contents of a file into a buffer
            virtual lfs_ssize_t file_read(lfs_file_t file, void *buffer, size_t size);

            // Write the contents of a buffer to a file
            virtual lfs_ssize_t file_write(lfs_file_t file, const void *buffer, size_t size);

            // Flush any buffers associated with the file
            virtual int file_sync(lfs_file_t file);

            // Move the file position to a given offset from from a given location
            virtual lfs_soff_t file_seek(lfs_file_t file, lfs_soff_t offset, int whence);

            // Get the file position of the file
            virtual lfs_soff_t file_tell(lfs_file_t file);

            // Get the size of the file
            virtual lfs_soff_t file_size(lfs_file_t file);

            // Open a directory on the filesystem
            virtual int dir_open(lfs_dir_t* dir, const char *path);

            // Close a directory
            virtual int dir_close(lfs_dir_t* dir);

            // Read the next directory entry
            virtual lfs_ssize_t dir_read(lfs_dir_t* dir, lfs_info *info);

            // Set the current position of the directory
            virtual void dir_seek(lfs_dir_t* dir, lfs_soff_t offset);

            // Get the current position of the directory
            virtual lfs_soff_t dir_tell(lfs_dir_t* dir);

            // Rewind the current position to the beginning of the directory
            virtual void dir_rewind(lfs_dir_t* dir);

        private:

            lfs_t lfs;
            //lfs_file_t file;

            //lfs_info info;

            struct lfs_config cfg;

            SpiNorFlash& spiNorFlash;

    };
  }
}


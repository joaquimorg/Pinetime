#include <cstring>
#include "FileSystem.h"
#include "SpiNorFlash.h"

#include "littlefs/lfs.h"

using namespace Pinetime::Drivers;

#define WRITEOFFSET 0x100000


////// Conversion functions //////
static int lfs_toerror(int err)
{
    return err;
}

////// Block device operations //////
static int lfs_bd_read(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    SpiNorFlash *spiNorFlash = (SpiNorFlash *)c->context;    
    spiNorFlash->Read(WRITEOFFSET + (block*c->block_size + off), static_cast<uint8_t *>(buffer), size);
    return 0;
}

static int lfs_bd_prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size) {
    SpiNorFlash *spiNorFlash = (SpiNorFlash *)c->context;    
    spiNorFlash->Write(WRITEOFFSET + (block*c->block_size + off), static_cast<const uint8_t *>(buffer), size);
    return 0;
}

static int lfs_bd_erase(const struct lfs_config *c, lfs_block_t block)
{
    //SpiNorFlash *spiNorFlash = (SpiNorFlash *)c->context;
    //return spiNorFlash->erase(block*c->block_size, c->block_size);
    // ????
    //for (size_t erased = 0; erased < c->block_size; erased += 0x1000) {
    //    spiNorFlash->SectorErase(WRITEOFFSET + (block*c->block_size) + erased);
    //}
    return 0;
}

static int lfs_bd_sync(const struct lfs_config *c)
{
    return 0;
}

FileSystem::FileSystem(SpiNorFlash& spiNorFlash) : spiNorFlash{spiNorFlash} {

    cfg.context = &spiNorFlash;

    // block device operations
	cfg.read  = lfs_bd_read;
	cfg.prog  = lfs_bd_prog;
	cfg.erase = lfs_bd_erase;
	cfg.sync  = lfs_bd_sync;

	// block device configuration
	cfg.read_size = 128;
	cfg.prog_size = 128;
	cfg.block_size = 4096;
	cfg.block_count = 1792;
    cfg.cache_size = 128;
    cfg.block_cycles = 500;
	cfg.lookahead_size = 128;    

}

int FileSystem::mount()
{
    // mount the filesystem
    int err = lfs_mount(&lfs, &cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        err = lfs_format(&lfs, &cfg);
        err = lfs_mount(&lfs, &cfg);
    }
    return 0;
}

FileSystem::~FileSystem()
{
    // nop if unmounted
    //unmount();
}

uint32_t FileSystem::read()
{
    // read current count
    uint32_t boot_count = 0;
    lfs_file_t lfs_file;
	
    lfs_file_open(&lfs, &lfs_file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &lfs_file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    lfs_file_rewind(&lfs, &lfs_file);
    lfs_file_write(&lfs, &lfs_file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &lfs_file);

    // release any resources we were using
    //lfs_unmount(&lfs);

    // print the boot count
    return boot_count;
}


int FileSystem::remove(const char *filename)
{
    //_mutex.lock();    
    int err = lfs_remove(&lfs, filename);    
    //_mutex.unlock();
    return lfs_toerror(err);
}

int FileSystem::rename(const char *oldname, const char *newname)
{
    //_mutex.lock();
    int err = lfs_rename(&lfs, oldname, newname);
    //_mutex.unlock();
    return lfs_toerror(err);
}

int FileSystem::mkdir(const char *name)
{
    //_mutex.lock();
    int err = lfs_mkdir(&lfs, name);
    //_mutex.unlock();
    return lfs_toerror(err);
}

int FileSystem::stat(const char *name, lfs_info *info)
{
    //struct lfs_info info;
    //_mutex.lock();
    int err = lfs_stat(&lfs, name, info);
    //_mutex.unlock();
    //st->st_size = info.size;
    //st->st_mode = lfs_tomode(info.type);
    return lfs_toerror(err);
}


////// File operations //////
int FileSystem::file_open(lfs_file_t *file, const char *path, int flags)
{
    //lfs_file_t *f = new lfs_file_t;
    //_mutex.lock();
    int err = lfs_file_open(&lfs, file, path, flags);
    //_mutex.unlock();
    /*if (!err) {
        *file = f;
    } else {
        delete f;
    }*/
    return lfs_toerror(err);
}

int FileSystem::file_close(lfs_file_t file)
{
    //lfs_file_t *f = (lfs_file_t *)file;
    //_mutex.lock();
    int err = lfs_file_close(&lfs, &file);
    //_mutex.unlock();
    //delete f;
    return lfs_toerror(err);
}

lfs_ssize_t FileSystem::file_read(lfs_file_t file, void *buffer, size_t len)
{
    //lfs_file_t *f = (lfs_file_t *)file;
    //_mutex.lock();
    lfs_ssize_t res = lfs_file_read(&lfs, &file, buffer, len);
    //_mutex.unlock();
    return lfs_toerror(res);
}

lfs_ssize_t FileSystem::file_write(lfs_file_t file, const void *buffer, size_t len)
{
    //lfs_file_t *f = (lfs_file_t *)file;
    //_mutex.lock();
    lfs_ssize_t res = lfs_file_write(&lfs, &file, buffer, len);
    //_mutex.unlock();
    return lfs_toerror(res);
}

int FileSystem::file_sync(lfs_file_t file)
{
    //lfs_file_t *f = (lfs_file_t *)file;
    //_mutex.lock();
    int err = lfs_file_sync(&lfs, &file);
    //_mutex.unlock();
    return lfs_toerror(err);
}

lfs_soff_t FileSystem::file_seek(lfs_file_t file, lfs_soff_t offset, int whence)
{
    //lfs_file_t *f = (lfs_file_t *)file;
    //_mutex.lock();
    lfs_soff_t res = lfs_file_seek(&lfs, &file, offset, whence);
    //_mutex.unlock();
    return lfs_toerror(res);
}

lfs_soff_t FileSystem::file_tell(lfs_file_t file)
{
    //lfs_file_t *f = (lfs_file_t *)file;
    //_mutex.lock();
    lfs_soff_t res = lfs_file_tell(&lfs, &file);
    //_mutex.unlock();
    return lfs_toerror(res);
}

lfs_soff_t FileSystem::file_size(lfs_file_t file)
{
    //lfs_file_t *f = (lfs_file_t *)file;
    //_mutex.lock();
    lfs_soff_t res = lfs_file_size(&lfs, &file);
    //_mutex.unlock();
    return lfs_toerror(res);
}


////// Dir operations //////
int FileSystem::dir_open(lfs_dir_t* dir, const char *path)
{
    //_mutex.lock();
    int err = lfs_dir_open(&lfs, dir, path);
    //_mutex.unlock();

    return lfs_toerror(err);
}

int FileSystem::dir_close(lfs_dir_t* dir)
{
    //_mutex.lock();
    int err = lfs_dir_close(&lfs, dir);
    //_mutex.unlock();

    return lfs_toerror(err);
}

lfs_ssize_t FileSystem::dir_read(lfs_dir_t* dir, lfs_info *info)
{

    //_mutex.lock();
    int res = lfs_dir_read(&lfs, dir, info);
    //_mutex.unlock();

    return lfs_toerror(res);
}

void FileSystem::dir_seek(lfs_dir_t* dir, lfs_soff_t offset)
{
    //_mutex.lock();
    lfs_dir_seek(&lfs, dir, offset);
    //_mutex.unlock();
}

lfs_soff_t FileSystem::dir_tell(lfs_dir_t* dir)
{
    //_mutex.lock();
    lfs_soff_t res = lfs_dir_tell(&lfs, dir);
    //_mutex.unlock();
    return lfs_toerror(res);
}

void FileSystem::dir_rewind(lfs_dir_t* dir)
{
    //_mutex.lock();
    lfs_dir_rewind(&lfs, dir);
    //_mutex.unlock();
}


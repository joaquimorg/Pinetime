
 
  External Flash MAP (4 MBytes)
  
  0x000000 +---------------------------------------+
           |  Bootloader Assets                    |
           |  256 KBytes                           |
           |                                       |
  0x040000 +---------------------------------------+
           |  OTA                                  |
           |  464 KBytes                           |
           |                                       |
           |                                       |
           |                                       |
  0x0B4000 +---------------------------------------+
           |  File System FAT                      |
           |  4 KBytes                             |
  0x0B5000 +---------------------------------------+
           |  File System FILES                    |
           |  3.328 MBytes                         |
           |                                       |
           |                                       |
           |                                       |
           |                                       |
  0x3F6000 +---------------------------------------+
           |  System Settings                      |
           |  40 KBytes                            |
  0x400000 +---------------------------------------+
 


------------------------------------------

flash sector is 4096 bytes
fat size is 1 sector

------------------------------------------

FAT 1 : 0x040000 - sector 0x40

FAT 2 : 0x041000 - sector 0x41 // not used

FILES : 0x042000 - sector 0x42

------------------------------------------

PINETIMEFS FAT
    typedef struct {
        uint8_t name[32]; // \0 terminated - size 32
        uint32_t offset; // size 4
        uint32_t size; // size 4
        uint8_t end = 0x00; // size 1
    } file_t;

    == total 41 bytes per file
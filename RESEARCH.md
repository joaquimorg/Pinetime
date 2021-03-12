LVGL spi Flash

https://github.com/noahossa/ender_v2_custom_Marlin/blob/b248cc94bbe19fccf192eedac7924a70e5afb109/Marlin/src/lcd/extui/lib/mks_ui/tft_lvgl_configuration.cpp
http://www.synwit.cn/Public/Uploads/2020-10-12/5f83f7a022863.pdf
https://blog.csdn.net/Mrkinte/article/details/110580302
https://www.codeleading.com/article/51225056187/



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
    


-- true color image size 240x240 = 115 205 kb

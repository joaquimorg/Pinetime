LVGL spi Flash

https://github.com/noahossa/ender_v2_custom_Marlin/blob/b248cc94bbe19fccf192eedac7924a70e5afb109/Marlin/src/lcd/extui/lib/mks_ui/tft_lvgl_configuration.cpp
http://www.synwit.cn/Public/Uploads/2020-10-12/5f83f7a022863.pdf
https://blog.csdn.net/Mrkinte/article/details/110580302
https://www.codeleading.com/article/51225056187/



------------------------------------------
//FIXED : less files, but can change only one
0x040000
    PINETIMEFS FAT 1 block 640 bytes
    
0x040400
    PINETIMEFS FILES (max 32 files fixed size 0x1DEE0) max file size = 122592 bytes -- ??? Fixed size or not

0x3FE000
    Settings = 0x002000 - 8192 bytes
0x400000

------------------------------------------

//variavel file size: more files, but need to send all fs if one size changes in the middle
// or can delete the changed file, move the next files over the delete one, and add the new at the end...
0x040000
    PINETIMEFS FAT 2 block 640 bytes = 1280
    
0x040500
    PINETIMEFS FILES (max 64 files)

0x3FE000
    Settings = 0x002000 - 8192 bytes
0x400000

------------------------------------------


PINETIMEFS FAT
    File name : uint8_t[11] - 11 bytes (exp. : "I_BG_CLOCK1" or "F_SYS_20   ")
    File offset : uint32_t - 4 bytes (exp. : 0x00040400)
    File size : uint32_t - 4 bytes (exp. : 0x0001C205)
    0x00 : space 1 byte

    == total 20 bytes per file
    32 files per block off 640 bytes

total free space = 0x3BDC00 = 3 922 944 bytes = 3 831 Mb
total free space = 0x3BDB00 = 3 922 688 bytes = 3 830 Mb

-- true color image size 240x240 = 115 205 kb

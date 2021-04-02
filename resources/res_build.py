#!/usr/bin/env python3

import os
from struct import pack

def concatFiles():
    fat = bytearray(b'\xFF') * 8176
    #bytearray(4096 * 2)
    path = "./resfiles"
    files = os.listdir(path)
    with open("pinetime-lite_1.0.2.res", "wb") as fo:
        # Write Header, file file identifiction 
        # 0x00 - File Identifiaction (0xAA)
        # 0x01 - File Type (RES-P-LITE)
        # 0x0B - File Version ( 0x0001 )
        # 0x0D - Resource Version ( 0x010001 ) ) 1.0.1
        fo.write(pack("16B", *bytearray([0xAA,0x52,0x45,0x53,0x2D,0x50,0x2D,0x4C,0x49,0x54,0x45,0x00,0x01,0x01,0x00,0x02])))
        fo.write(fat)
        file_offset = 0
        file_fat_offset = 0x10
        for infile in files:
            if infile.endswith('.bin'):
                fsize = os.path.getsize(os.path.join(path, infile))
                print('Adding file [' + infile + '] to resource, size : ' + str(fsize) + ', offset : ' + hex(file_offset))
                fo.seek(file_fat_offset)
                # File Fat header
                # name[32]
                # offset[4]
                # size[4]
                # end[1] = 0x00
                fo.write(pack("32sIIB", infile.encode('UTF-8'), file_offset, fsize, 0x00))
                file_offset = file_offset + fsize
                file_fat_offset = file_fat_offset + 0x2C
                fo.seek(0, 2)                
                with open(os.path.join(path, infile), "rb") as fin:
                    fo.write(fin.read())


print('\n\nPineTime Lite - Resource builder. - (c)2021 joaquim.org')
print('This programa builds the resource file to use in PineTime Lite.')
print('All ".bin" files in [resfiles] folder will be included in the resource file.')
print('This resource can only have a max of 90 files !')
concatFiles()

print('Done.');
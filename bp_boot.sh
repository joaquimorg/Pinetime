#!/bin/sh
set -e
make -j pinetime-mcuboot-lite
cd ../bootloader
./create_image.sh
cp pinetime.bin /mnt/d/Tools/xpack-openocd-0.10.0-15/bin/pinetime_lite_0.1.0.bin
#arm-none-eabi-gdb.exe --batch -ex="target extended-remote 192.168.1.20:3333" -ex "monitor program ./pinetime_lite_0.1.0.bin verify 0x00008000" -ex "monitor reset"
## nrf5 mass_erase
## program bootloader.bin 0
## program pinetime_lite_0.1.0.bin verify 0x8000
cd ../build
#program ./image.bin verify 0x00008000
:
#-ex "q"

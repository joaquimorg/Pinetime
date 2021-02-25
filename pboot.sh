#!/bin/sh

echo "\nErase Internal flash and program Bootloader to 0x0\n"

set -e
arm-none-eabi-gdb --batch -ex="target extended-remote 192.168.1.20:3333" -ex "monitor halt" -ex "monitor nrf5 mass_erase" -ex "monitor program d:/Work/PineTime/OS/Pinetime/bootloader/pinetime-bootloader-rc4.bin 0x00000000"
:
#-ex "q"

#!/bin/sh

echo "\nProgram to 0x00000000\n"

#arm-none-eabi-gdb --batch \
#-ex="target extended-remote 192.168.1.20:3333" \
#-ex "monitor program d:/Work/PineTime/OS/PinetimeLiteFS/build/output/pinetime-lite-0.1.0.bin 0x00008000" \
#-ex "monitor reset"

arm-none-eabi-gdb.exe --batch -ex="target extended-remote 192.168.1.20:3333" -ex "load" -ex "monitor reset" build/src/pinetime-lite-0.1.0.hex


#!/bin/sh
echo "\nProgram to 0x0\n"

arm-none-eabi-gdb --batch -ex="target extended-remote 192.168.1.20:3333" -ex "monitor program d:/Work/PineTime/OS/PinetimeQ3New/build/output/src/pinetime-lite-0.1.0.hex 0x0000000" -ex "monitor reset"
#arm-none-eabi-gdb --batch -ex="target extended-remote 192.168.1.20:3333" -ex "monitor reset"


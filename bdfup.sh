#!/bin/sh

echo "\nBuild pinetime-lite and Program to 0x00008000\n"

set -e
docker run --rm -it -v $(pwd):/sources pinetime-build
arm-none-eabi-gdb --batch -ex="target extended-remote 192.168.1.20:3333" -ex "monitor program d:/Work/PineTime/OS/Pinetime/build/output/pinetime-lite-0.1.0.bin 0x00008000" -ex "monitor reset"
:
#-ex "q"

#!/bin/sh
set -e
make -j pinetime-lite
arm-none-eabi-gdb.exe --batch -ex="target extended-remote 192.168.1.20:3333" -ex "load" -ex "monitor reset" src/pinetime-lite-0.1.0.hex
:
#-ex "q"

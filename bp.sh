#!/bin/sh
set -e
make -j pinetime-app
arm-none-eabi-gdb.exe --batch -ex="target extended-remote 192.168.1.20:3333" -ex "load" -ex "monitor reset" -ex "det" src/pinetime-app-0.10.1.hex
:
#-ex "q"

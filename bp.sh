#!/bin/sh
set -e
make -j pinetime-app
arm-none-eabi-gdb.exe -ex="target remote 192.168.1.187:3333" src/pinetime-app-0.9.0.out -ex "load" -ex "det" -ex "q"
:

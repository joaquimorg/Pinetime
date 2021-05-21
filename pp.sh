#!/bin/sh
echo "\nBuild pinetime-lite and Program to 0x0\n"

set -e
#docker run --rm -it -v $(pwd):/sources pinetime-build /opt/build.sh pinetime-lite
arm-none-eabi-gdb.exe --batch -ex="target extended-remote 192.168.1.20:3333" -ex "load" -ex "monitor reset" build/src/pinetime-lite-0.6.5.hex

# using BlackMagic
#arm-none-eabi-gdb.exe --batch -ex="target extended-remote COM5" -ex "attach 1" -ex "load" build/src/pinetime-lite-0.6.5.hex
:

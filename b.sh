#!/bin/sh
#make -j pinetime-lite
#arm-none-eabi-gdb.exe --batch -ex="target extended-remote 192.168.1.20:3333" -ex "load" -ex "monitor reset" src/pinetime-lite-0.1.0.hex
echo "\nBuild pinetime-lite\n"

set -e
docker run --rm -it -v $(pwd):/sources pinetime-build /opt/build.sh pinetime-lite
#arm-none-eabi-gdb --batch -ex="target extended-remote 192.168.1.20:3333" -ex "load" -ex "monitor reset" build/src/pinetime-lite-0.1.0.hex
:

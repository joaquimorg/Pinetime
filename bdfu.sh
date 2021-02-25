#!/bin/sh

echo "\nBuild pinetime-lite and generate DFU\n"

set -e
docker run --rm -it -v $(pwd):/sources pinetime-build
:
#-ex "q"

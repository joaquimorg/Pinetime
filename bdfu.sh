#!/bin/sh

echo "\nBuild pinetime-lite and generate DFU\n"

docker run --rm -it -v $(pwd):/sources pinetime-build



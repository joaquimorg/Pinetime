#!/bin/bash

/mnt/d/Work/PineTime/OS/mcuboot/scripts/imgtool.py  create --align 4 --version 1.0.0 --header-size 32 --slot-size 475136 --pad-header ../build/src/pinetime-mcuboot-app-0.9.0.bin image.bin
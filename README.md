# PineTime

![Build PineTime Firmware](https://github.com/joaquimorg/Pinetime/workflows/Build%20PineTime%20Firmware/badge.svg)

> The PineTime is a free and open source smartwatch capable of running custom-built open operating systems. Some of the notable features include a heart rate monitor, a week-long battery as well as a capacitive touch IPS display that is legible in direct sunlight. It is a fully community driven side-project, which means that it will ultimately be up to the developers and end-users to determine when they deem the PineTime ready to ship.

> We envision the PineTime as a companion for not only your PinePhone but also for your favorite devices — any phone, tablet, or even PC.

*https://www.pine64.org/pinetime/*

The **Pinetime** smartwatch is built around the NRF52832 MCU (512KB Flash, 64KB RAM), a 240*240 LCD display driven by the ST7789 controller, an accelerometer, a heart rate sensor, and a vibration motor.

# PinetimeLite

This project is a fork of [Infinitime](https://github.com/JF002/InfiniTime)

The purpose of **PinetimeLite** is to build firmware for Pinetime that is lightweight and maximizes battery life.
The functionalities to be implemented are:
- Clock, with the possibility of having several faces.
- Notifications, both from applications such as SMS, Whatsapp, etc ... as well as calls.
- Step count

All other features available on the Pinetime hardware will not be supported.

## Licenses
This project is released under the GNU General Public License version 3 or, at your option, any later version.

## Screenshots

![Pinetime](images/screens/pinetime_3828.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3847.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3857.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3818.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3905.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3915.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3922.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3930.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3939.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3947.jpg "Pinetime")
![Pinetime](images/screens/pinetime_3955.jpg "Pinetime")
![Pinetime](images/screens/pinetime_4004.jpg "Pinetime")
![Pinetime](images/screens/pinetime_2745.jpg "Pinetime")
![Pinetime](images/screens/pinetime_4019.jpg "Pinetime")
![Pinetime](images/screens/pinetime_4028.jpg "Pinetime")
![Pinetime](images/screens/pinetime_4044.jpg "Pinetime")
![Pinetime](images/screens/pinetime_4106.jpg "Pinetime")
![Pinetime](images/screens/pinetime_4113.jpg "Pinetime")

[![PineTime Lite 0.1.0](https://res.cloudinary.com/marcomontalbano/image/upload/v1615222388/video_to_markdown/images/youtube--vJQQdbcaj8U-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://www.youtube.com/watch?v=vJQQdbcaj8U "PineTime Lite 0.1.0")

## Screen Mockups

https://www.figma.com/file/Wx1Z5mz2IgCbQDQS8r0Ljr/Pinetime-Screens-v0.1?node-id=0%3A1

![Pinetime screen mockup](images/PinetimeClockMockup.png "Pinetime")

----
## Build PinetimeLite

- $ gh repo clone joaquimorg/PinetimeLite
- $ git submodule update --init --recursive

- $ mkdir build
- $ cd build

### Config

$ cmake -DCMAKE_BUILD_TYPE=Release -DARM_NONE_EABI_TOOLCHAIN_PATH=/usr -DNRF5_SDK_PATH=/mnt/d/Work/PineTime/nRF5_SDK_15.3.0_59ac345 -DUSE_OPENOCD=1 -DOPENOCD_BIN_PATH=/mnt/d/Tools/xpack-openocd-0.10.0-15/bin/openocd.exe ../

##### Sync time on WSL2
$ sudo hwclock -s

##### regular build
$ make -j pinetime-lite

##### mcboot build
$ make -j pinetime-mcuboot-lite

##### build and upload script
$ bp.sh

##### upload via remote openocd

$ arm-none-eabi-gdb.exe --batch -ex="target extended-remote 192.168.1.20:3333" -ex "load" -ex "monitor reset" src/pinetime-lite-0.0.1.out


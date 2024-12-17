
# firmware-upgrade-L496
Repo with firmware upgrade and custom bootloader for STM32L496ZG

# Prerequisites
 1. Insall ARM GCC Toolchain
 2. Openocd (version : 0.11.0-2-win32-x64)
 3. Cmake
 4. Make
# Building
 ` $env:Path += ";C:\MinGW\bin;C:\Program Files\CMake\bin;C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10\bin"  `

 ` mkdir build && cd build `

 Generate cmake files
 `cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug -DNDEBUG=0 ..`

For windows

 `cmake -G "Unix Makefiles" --toolchain ../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug -DNDEBUG=0 ..`

FOR Production builds use flag NDEBUG=0
FOR Test Builds use flag NDEBUG=1

 Building

 `cmake --build . -- -j12`

# Flashing

For windows
    `./flash_all.bat`
For Linux
    :TODO

FLAG Partition
FLAG | BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7
--- | --- | --- | --- | --- |--- |--- |--- |---
USE | CHANGE PARTITION |

FLAG | BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15
--- | --- | --- | --- | --- |--- |--- |--- |---
USE |

FLAG | BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23
--- | --- | --- | --- | --- |--- |--- |--- |---
USE |  |


FLAG | BIT24 | BIT25 | BIT26 | BIT27 | BIT28 | BIT29 | BIT30 | BIT31
--- | --- | --- | --- | --- |--- |--- |--- |---
USE |

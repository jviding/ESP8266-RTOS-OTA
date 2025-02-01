# ESP8266-RTOS-OTA

Remote OTA firmware updates with FreeRTOS on ESP8266.

## Installations

1) Toolchain: xtensa-lx106-elf
2) ESP8266 libs: ESP8266_RTOS_SDK

For more details:
https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html

Set ENV for make:
>$ export PATH="$PATH:$HOME/esp/xtensa-lx106-elf/bin"  <br>
>$ export IDF_PATH="$HOME/esp/ESP8266_RTOS_SDK"

APIs, libs, build conf:
https://github.com/espressif/ESP8266_RTOS_SDK/blob/master/docs/en/api-guides/build-system.rst

## Project configuration

>$ make menuconfig

## Build

>$ make          <br>
>$ make all      <br>
>$ make clean    <br>
>$ make flash

## Flash

With esptool:
>$ esptool.py --chip esp8266 --port /dev/ttyUSB0 write_flash -z \\  <br>
>   0x00000 bootloader.bin \\        <br>
>   0x10000 esp8266-rtos-ota.bin \\  <br>
>   0x8000 partitions.bin

Or /dev/ttyS0 with serial (TX,RX)

## Debug

Read serial:
>$ minicom -D /dev/ttyUSB0 -o -b 115200

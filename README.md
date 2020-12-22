# nrf-test-tool
IRNAS nRF test tool for programming, testing and power measurement

## System features
* IRNAS nRF-test-tool board
* 4 channel FTDI UART
* Nordic PPK2 power profiling kit
* Nordic nRF9160DK as programmer and control
* Support for up to 4 targets per tool
* JTAG connection switching, 2 channels per target
* Power measurement of each individual target (one or multiple from single source)
* Analog measurement channels
* Reset channels
* Python API for control

## Key user features
* Turn on/off each target device
  * Select power source for each target as PPK2 or regulator
* Route JTAG to each target device + select between two ports on each device, disconnect programming pins after programming
* Measure analog voltages
* Trigger actions via push buttons
* See device status with LED
* Reset each device


## Requirements
* nrf9160dk board
* NCS v1.3.0

## Special note about NRF9160DK

In order to run this library on NRF9160 development board, we have to flash onboard NRF52840 with certain GPIO configuration as this chip controls GPIO routing of NRF91. 

* We have to reroute pins: P0.02 - P0.09 to headers.
* Hex file (`nrf52840-pin-reroutes.hex`) was already generated for this purpose
* Flip the switch PROG/DEBUG on dev kit to nrf52
* Then flash using the following command: `west flash`

If above .hex file doesn't work, the following steps need to be done:
* Move into `zephyr/samples/hello_world` folder
* Execute `west build -p -b nrf9160dk_nrf52840`
* Execute `west build -t menuconfig`
* Select option `Board options`
* For each of listed options select option to use them with Arduino headers: UART1, Button 2, LED 2, LED 4
* Press Q and confirm save
* Execute `west build -b nrf9160dk_nrf52840 && west flash`

NRF52 chip is now correctly configured and switch PROG/DEBUG can be flippend back to nrf91.

## Commands to build and run Zephyr code
1. `cd zephyr`
1. `west build -p auto -b nrf9160dk_nrf9160ns`
2. `west flash`

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

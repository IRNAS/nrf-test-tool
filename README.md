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
* To send data to grafana install: `python3 -m pip install influxdb`

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
* Lock nrf91 with `nrfjprog --rbp ALL`

NRF52 chip is now correctly configured and switch PROG/DEBUG can be flippend back to nrf91.

## Commands to build and run Zephyr code
1. `cd zephyr`
1. `west build -p auto -b nrf9160dk_nrf9160ns`
2. `west flash`

## API commands
The board is controlled by the following UART commands with parameters:
```
COMMAND                     DESCRIPTION                                VALID VALUES

power <target> <state>      Set power on selected target               <target>: 0-3 - target 0 to 3
                                                                       <state>: "on" - power target from board, "off" - disable power, "ppk" - power target from ppk2

jtag <ch_num>               Enable selected jtag channel               <ch_num>: 0-7 or -1 - see JTAG Channel Numbering

reset <ch_num>              Reset selected jtag channel                <ch_num>: 0-7 - see Reset Channel Numbering

adc <ch_num>                Read analog value from selected channel    <ch_num>: 0-15 - see Adc Channel Numbering

led <ch_num> <state>        Set LED state on selected channel          <ch_num>: 0-8 - see LED Channel Numbering
                                                                       <state>: "on" - enable LED, "off" - disable LED
```

## Channel numbering
### JTAG and Reset Channel Numbering
Each target (0 to 3) has access to two chips: NRF52 on line 0 and NRF91 on line 1. To select the chip use `jtag <ch_num>` and reset the chip with `reset <ch_num>`.

```
CHANNEL <ch_num>    SELECTED CHIP

-1                  None - turn all programmers off (jtag parameter only)

0                   NRF52 on target 0
1                   NRF91 on target 0

2                   NRF52 on target 1
3                   NRF91 on target 1

4                   NRF52 on target 2
5                   NRF91 on target 2

6                   NRF52 on target 3
7                   NRF91 on target 3
```

### ADC Channel Numbering
Each target (0 to 3) has it's own adc with four Analog In channels.
```
CHANNEL <ch_num>    ANALOG IN

0                   TEST1 on target 0
1                   TEST2 on target 0
2                   TEST3 on target 0
3                   TEST4 on target 0

4                   TEST1 on target 1
5                   TEST2 on target 1
6                   TEST3 on target 1
7                   TEST4 on target 1

8                   TEST1 on target 2
9                   TEST2 on target 2
10                  TEST3 on target 2
11                  TEST4 on target 2

12                  TEST1 on target 3
13                  TEST2 on target 3
14                  TEST3 on target 3
15                  TEST4 on target 3
```

### LED Channel Numbering
Each target (0 to 3) has two correspoding LED's. A green LED on line 0 and a red LED on line 1. The LED's can be turned on/off based on `<state>`.
```
CHANNEL <ch_num>    LED

0                   Green LED on target 0
1                   Red LED on target 0

2                   Green LED on target 1
3                   Red LED on target 1

4                   Green LED on target 2
5                   Red LED on target 2

6                   Green LED on target 3
7                   Red LED on target 3
```

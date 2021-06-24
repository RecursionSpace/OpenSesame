# Arduino

This version of OpenSesame has been developed to work on the [ATmega328P](https://www.microchip.com/wwwproducts/en/ATmega328P)

## PINS

| Digital |                    | Analog |               |
|:--------|--------------------|:-------|---------------|
| D0      | RX Programming Pin | A0     | System Status |
| D1      | TX Programming Pin | A1     |               |
| D2      | Wiegand Low        | A2     |               |
| D3      | Wiegand High       | A3     |               |
| D4      | Wiegand LED        | A4     |               |
| D5      | DoorA Trigger      | A5     |               |
| D6      | Button Signal      | A6     |               |
| D7      | Motion Signal      | A7     |               |
| D8      | XBee TX            |        |               |
| D9      | XBee RX            |        |               |
| D10     | SS                 |        |               |
| D11     | MOSI               |        |               |
| D12     | MISO               |        |               |
| D13     | SCK                |        |               |

## Memory Card (MicroSD)

In order to store data locally and provide easy user configuration, a MicroSD card is used. The card is formatted to be in FAT16 file system

The memory card is pre-loaded with a configuration file that is produced by the control hub. The configuration file contains the following information:
XBee Pan ID
XBee Encryption Key
User Defined Node Name
Delay times for exit and entrance
Initial data dump

The additional hardware is needed for the MicroSD card includes the following
Card adapter
Logic Buffer

Schematic based on tested adapter from amazon and reference blog.

Communication with the MicroSD card is accomplished with the Serial Peripheral Interface (SPI) library.

# OpenSesame

OpenSesame provides an application programming interface (API) between the access control hardware and the access management system. The flexability of OpenSesame accepts a range of authentication input methods with a defined authorization method.

# Table of Contents
- [Compatible Controllers](#compatable-controllers)
- [Community](#community)


| Protocall | Function                                | Status                  |
|-----------|-----------------------------------------|-------------------------|
| Wiegand   | A de facto wiring standard for readers. | Supported               |
| OSDP      | Open Source Device Protocol             | Integration In Progress |

# Compatable Controllers

While OpenSesame can run on prototype/hobby hardware platform a custom board will increase reliability in production environments and improve the instalation process.

### Supported Hardware<sup>1</sup>

| Controller Name | Producer        | URL                                                      | Verified<sup>2</sup> |
|-----------------|-----------------|----------------------------------------------------------|:--------------------:|
| 🌱 Sesame Seed  | Recursion.Space | [recursion.space/sesameseed](recursion.space/sesameseed) |          ✓           |
|                 |                 |                                                          |                      |

_[1] To add supported hardware create a pull request with the required table information. <br>
[2] Verified boards have been tested by Recursion.Space with the latest version of OpenSesame to validate functionality. To submit hardware for verification email info@recursion.space_

# Microcontrollers

## Arduino
OpenSesame_Arduino contains an Arduino compatable project with an .ino file.

## ESP32
TO DO - Port the Arduino code over to become compatable with the ESP32 controller.

# Continuous Integration

All code will need to follow the brest practices and standards outlined by [MISRA](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx)

# Community

<div align="center">

<a target="_blank" href="https://discord.com/invite/KnFp4jd9AV">![Discord Banner 2](https://discordapp.com/api/guilds/790311269420630079/widget.png?style=banner2)</a>

</div>

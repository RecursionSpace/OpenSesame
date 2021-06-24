<center> <h1> OpenSesame </h1></center>

# Table of Contents
- [Table of Contents](#table-of-contents)
- [What is OpenSesame?](#what-is-opensesame)
- [Supported Protocols](#supported-protocols)
- [Supported Controllers](#supported-controllers)
- [Install](#install)
- [Continuous Integration](#continuous-integration)
- [Community and Contributing](#community-and-contributing)
- [Directory Structure](#directory-structure)
- [Licensing](#licensing)

---
<br>

# What is OpenSesame?

[OpenSesame](https://github.com/RecursionSpace/OpenSesame) is an open source access control system. Currently, OpenSesame provides an application programming interface (API) between the access control hardware and the access management system. The flexability of OpenSesame accepts a range of authentication input methods with a defined authorization method.

# Supported Protocols

OpenSesame integrates with the most popular RFID readers that use industry standard communication protocols including:
- Wiegand<sup>*</sup>
- Open Source Device Protocol (OSDP)

_<sup>*</sup>Included for legacy integration, not recommended for new installations due to known security flaws._

# Supported Controllers

While OpenSesame can run on prototype/hobby hardware platform a custom board will increase reliability in production environments and improve the instalation process.

| Controller<sup>1</sup> | Producer        | URL                                                      | Verified<sup>2</sup> |
|------------------------|-----------------|----------------------------------------------------------|:--------------------:|
| 🌱 Sesame Seed         | Recursion.Space | [recursion.space/sesameseed](recursion.space/sesameseed) |          ✓           |
|                        |                 |                                                          |                      |

_[1] To add supported hardware create a pull request with the required table information. <br>
[2] Verified boards have been tested by Recursion.Space with the latest version of OpenSesame to validate functionality. To submit hardware for verification email info@recursion.space_

# Install

At the moment OpenSesame is natively compatable for an Arduino environments. After cloning the repository, open OpenSesame_Arduino.ino and upload to a board.

The next microcontroller to be supported will be the ESP32.

# Continuous Integration

All code will need to follow the brest practices and standards outlined by [MISRA](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx)

# Community and Contributing

OpenSesame is developed by [Recursion.Space](https://recursion.space/) and by users like you. We welcome both pull requests and issues on [GitHub](https://github.com/RecursionSpace/OpenSesame). Bug fixes and new protocols are encouraged.

<div align="center">

<a target="_blank" href="https://discord.com/invite/KnFp4jd9AV">![Discord Banner 2](https://discordapp.com/api/guilds/790311269420630079/widget.png?style=banner2)</a>

</div>

# Directory Structure

```
.
├── docs                # Design guidelines and subsystem readmes
└── OpenSesame_Arduino  # Main code compatable with the Arduino environment.
    └── src             # OpenSesame specific libraries.
```

# Licensing

OpenSesame is released under the MIT license.

---

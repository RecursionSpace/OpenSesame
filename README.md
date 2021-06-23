# OpenSesame

OpenSesame is an open source access management system. OpenSesame sits between a central database and the doors electronic locks.

# Definitions

# Means of Egress
: The term “means of egress” is defined in the International Building Code (IBC) as “A continuous and unobstructed path of vertical and horizontal egress travel from any occupied portion of a building or structure to a public way.”

The Recursion Egress Controller is the intermediary device between a central database manager and the electronic door interfaces. The controller is designed to ensure a maximum level of reliability and uptime, the primary function of the controller is to restrict unwanted access while ensuring the safe exit in the case of an emergency.

The controller has been designed to not only ensure ease of use from the end customer but to the technician that will be installing the controller. The design should allow the end user to be confident and proud that they have a Recursion product, by keeping in mind the technician that has to ultimately perform the installation the design should allow for a clean finish.

Each controller is designed to function with the input of a single card reader, and control either one (1) or two (2) doors. The following chart goes over additional technical specifications of the controller:

### Egress Controller Technical Specifications
| Specification                   | Requirement           |
|---------------------------------|-----------------------|
| Input Voltage                   | 12v - 24v             |
| Lock Operational Voltage        | Matches Input Voltage |
| Door Position Switch Input      | Yes                   |
| Push to Exit Button Input       | Yes                   |
| Secondary Request to Exit Input | Yes                   |
| Remote Request to Exit          | Software Controlled   |

# Electronic Access Control
: Access is not regulated by national building codes except in the case of high rise stair towers where re-entry must be possible to get to the other stair tower in the event of smoke. The AHJ is not concerned with who enters; only that they must be able to exit at will. Early efforts to lock people in and require that they “card out” were illegal and unsafe. They have since been replaced with three other methods: delayed egress, as discussed previously; anti-passback, a method allowing free egress, but restricting future access if a card, code or other qualifier is not presented prior to egress; and embarrassment alarms, also free egress, but drawing attention to an unqualified egress.

# BOCA, ICBO (UBC) and SBCCI
: the model code groups that merged to form the IBC in 2000. They are referred to as legacy codes and have not been active since IBC presented.

# NFPA
: is the set of model codes from which virtually all fire codes are based. As a code, NFPA 101 is primarily used in federally regulated buildings. NFPA 101 is used by CMS and The Joint Commission to inspect virtually all hospitals. GSA uses NFPA. GSA is the authority having jurisdiction in federally owned buildings on federally owned property.

# IBC
: The model building code used in most states and municipalities. An understanding of the sections titled Use and Occupancy and Means of Egress are important. Following adoption of a new code, many jurisdictions publish a list of changes to the code pertaining to their jurisdiction.



# Hardware (Control Boards)

While OpenSesame can run on prototype/hobby hardware platform a custom board will increase reliability in production environments and improve the instalation process.

### Supported Hardware<sup>1</sup>

| Controller Name | Producer        | URL                                                      | Verified<sup>2</sup> |
|-----------------|-----------------|----------------------------------------------------------|:--------------------:|
| Sesame Seed     | Recursion.Space | [recursion.space/sesameseed](recursion.space/sesameseed) |          ✓           |
|                 |                 |                                                          |                      |

_[1] To add supported hardware create a pull request with the required table information. <br>
[2] Verified boards have been tested by Recursion.Space with the latest version of OpenSesame to validate functionality. To submit hardware for verification email info@recursion.space_

# IDE

To lower participation barriers the Arduino IDE is supported with the use of .ino files.

# Continuous Integration

All code will need to follow the brest practices and standards outlined by [MISRA](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx)

# Community

<div align="center">

<a target="_blank" href="https://discord.com/invite/KnFp4jd9AV">![Discord Banner 2](https://discordapp.com/api/guilds/790311269420630079/widget.png?style=banner2)</a>

</div>

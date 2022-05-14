## Light Sensor System for the Hoosier National Forest

> This repository houses the code, circuit schematics, and 3D models for the light sensor system.

> The light sensor system is an autonomous, ultra-low power embedded microcontroller system designed to take light-level readings from the night sky. Over the course of hte next year, it will be deployed in teh Hoosier National Forest to collect sensor readings for the Forest Service in their attempt to gain 'dark sky' status.

> The software utilized by the system is an interrupt driven program that utilized the system's Real-Time Clock (RTC) to generate regular interrupts at fifteen minute intervals. These intervals are generated between the times of 8:00pm and 8:00am, daily, over the course of the system's one year lifetime so that the sensor system is able to capture how low the light levels reach over the course of each night. These readings are then stored bythe software in the system's FLASH memory back for collection at the end of the device's lifetime.

> TGhe electrical system, responsible for powering the system's CPU and peripheral devices, maximizes its operational life cycle by switching between a high powered running mode used for taking readings and low power mode which conserves the state of the machine whilst still limiting the expense of power. Utilizing the modes of operation that we do, the maximum lifetime of the light sensor system has been estimated to be three years using the STMCube software, allowing confidence that the system can operate for a year without issue.

> As part of the overall system, a 3D printable, water-resistant enclosure has been designed to house the electrical components. An enclosure such as the one we designed is a necessary part of the larger system not only because it will shield the PCB and its components from external forces such as dirt and rain while it is out in the forest, but it will also protect wildlife from damaging the electrical system and vice versa. The enclosure has a securable lid for easy access to the PCB and a port on the bottom of the enclosure for access to a 3-pin connector for later collection of the data captured by the light sensor system.

> The system has been designed to be as low cost as possible such that it may be produced en masse for the Forest Service so that they might deploy the devices throughout the 200,000 acres of the forest and maximize their chances to collect the low-light level readings that they need.

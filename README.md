This project is meant to turn any Arduino or clone into a powerful I2C motor controller with dual encoder support. It supports a variety of H-Bridge chips and modules as long as they use they use two direction pins and one PWM pin per channel. Most analog motor driver chips you can find work like that. 

Here are my recommendations:

* **L298** You can usually find breakout boards for these dual 2A motor drivers at around 2-5$ everywhere around the world. Here's an [example](https://www.aliexpress.com/item/1005001621936295.html) from Aliexpress. They make a great pair with TT style motors, and other low-current options. However these chips are very sensitive to loads over the 2A limit.

* **TB6612FNG**
	At arount the same price as the L298, [these modules](https://shop.pimoroni.com/products/tb6612fng-dual-motor-driver-carrier
) have a higher peak current of 3.2A, making them a great to pair with Pololu Micro Motors / N20 Motors. 
	
* **VNH30x Series**
	Originally designed for powersteering and other automotive uses, the is a great option for large(r) motors. Each chip can support up to 15A and 30A peak loads on one channel. Additionally these chips have an analog output pin called **CS**, or **C**urrent **S**ense. You can read the voltage on this pin to see how much current each motor uses. This will be supported in later versions of the firmware. There are a few breakout boards on the market, such as [this one](https://www.pololu.com/product/707) by Pololu.

Motorhead is also available as a package for robot-block, offering a plug-and-play ROS experience.


## Variables
To work with a variety of different boards, some specifics need to be configured using variables. 

### Build Variables 
To compile the arduino sketch at build-time, some information about your specific microcontroller is needed. 
Build variables can be defined in `balena.yml` inside the `.balena` folder.  

| Variable | Description | Example | 
| ----------- | ----------- |----------- |
| `ARDUINO_CORE` | The microcontroller family your board is based on | `arduino:avr` |
| `ARDUINO_FQBN` | Board identification as Follows the following naming convention. `VENDOR:ARCHITECTURE:BOARD_ID[:MENU_ID=OPTION_ID[,MENU2_ID=OPTION_ID ...]]` | `arduino:avr:pro:cpu=8MHzatmega328` |

### Runtime Variables
These variables can be defined on the dashboard, or in the docker compose. 

| Variable | Description | Example | 
| ----------- | ----------- |----------- |
| `AVRDUDE_CPU` | Name of the microcontroller your board is based on | `atmega328p` |
| `MONITOR_BAUD` | Baud rate for miniterm (after flashing, the arduino service opens a serial terminal for monitoring) | `9600` |
| `SERIAL_PORT` | Serial port your board is connected to. | `/dev/ttyUSB0` |


## I2C Registers
#### Mode Configuration
| Register | Description | R/W |
| ----------- | ----------- |----------- |
| 0xFF | I2C Mode | R/W 
| 0xFD | UART Mode | R/W 
#### Pin Configuration
| Register | Description | R/W |
| ----------- | ----------- | ----------- |
| 0x00 | Left Motor PWM | R/W |
| 0x01| Left Motor A | R/W |
| 0x02| Left Motor B | R/W |
| 0x03| Left Encoder A | R/W |
| 0x04| Left Encoder B | R/W |
| 0x05| Right Motor A | R/W |
| 0x06| Right Motor B | R/W |
| 0x07| Right Motor PWM | R/W |
| 0x0A| Right Encoder A | R/W |
| 0x0B| Right Encoder B | R/W |

#### Motors

##### Motor Left
| Register | Description | R/W |
| ----------- | ----------- | ----------- |
| 0x10 | Speed  | R/W |
| 0x11| Direction | R/W |
| 0x12| Go |W |

##### Motor Right
| Register | Description |  R/W |
| ----------- | ----------- | ----------- |
| 0x20 | Speed  | R/W |
| 0x21| Direction | R/W |
| 0x22| Go |W |

#### Encoders
| Register | Description |  R/W |
| ----------- | ----------- | ----------- |
| 0x30| Count Left  | R/W |
| 0x31| Count Right  |  R |

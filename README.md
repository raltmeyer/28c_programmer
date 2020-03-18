# 28c_Programmer

2020 - Rogerio Altmeyer

[Versão em Português](https://github.com/raltmeyer/28c_programmer/README_ptbr.md)

## Basic description

This EEPROM programmer was born to support the development of another retro-computing project that I'm working on, where the focus is to recreate computers from the 80's from scratch in a modular construction and compatible with the original binaries.

The recorder was created with theATMel's 32Kb EEPROM AT28C256 in mind, and at this time firmware is only compatible with this chip, and can be easily converted to other parallel memories.

## Inside the project

The project is divided into three parts:
- Schematic design of the circuit and design of the PCB board (Hardware).
- Firmware for the board (C ++)
- Client that runs on the host machine responsible for communicating with the board (Python)


## Circuit board

The board uses the Arduino Nano as the main component, responsible for communicating with the host computer and programming the memory chip.

The control bus and the data bus are connected directly to the Nano, and the 15 addressing lines of the chip are fed through two 8-bit shift registers connected in sequence.

Schematic for the Board V1.3
![chematic for the Board V1.3](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/Schematic_eeprom28_programmer.png)

The board design is available in [ZIP with Board Gerber](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/Gerber_eeprom28_programmer.zip). 
I used the services from [JLCPCB.com](https://jlcpcb.com) and recommend it.

## Firmware da placa

O firmware da placa foi desenvolvido em C++ no Eclipse IDE, e traz como característica principal a implementação de interpretador de mensagens vindas do computador host. Com o interpretador de mensagens, foi possível organizar e facilitar a codificação de novas funcionalidades ao firmware.

## Board firmware

The board's firmware was developed in C++ using the Eclipse IDE, and its main feature is the implementation of a message interpreter from the host computer. With the message interpreter, it was possible to organize the code for new features to the firmware.

### Firmware Core
- [28c_programmer.h](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/28c_programmer.h) Main header
- [pin_defs.h](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/pin_defs.h) Header with pins definitions
- [main.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/main.cpp) Main source containing the functions setup() e loop()
- [interface.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/interface.cpp) Contains all the functions for the interface between Arduino and EEPROM
- [util.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/util.cpp) Miscellaneous code support
- [cmd_processor.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_processor.cpp) Implements the message interpreter

### Functions for use by the host
- [cmd_clear_block.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_clear_block.cpp) Function to clear EEPROM
- [cmd_get_version.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_get_version.cpp) Function that returns the card's firmware version
- [cmd_read_block.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_read_block.cpp) Function that reads a 16-byte block and returns it to the host
- [cmd_read_byte.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_read_byte.cpp) Function that reads 1 byte
- [cmd_write_block.cpp](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_firmware/cmd_write_block.cpp) Function that writes from 1 to 16 bytes to the EEPROM

## Command line client for the host computer

Written in Python3, this layer is responsible for read/write files on the host machine and send/receive to/from the board's firmware.

### Dependency installs (macOS / Linux)

To run the client, you must have `python3` and install the following dependencies:
```
pip3 install argparse
pip3 install pyserial
```

You will also need to adjust the permissions:
```
chmod +x programmer.py
```

### Supported parameters
The client will show the following help indicating the supported parameters:
```
#./programmer.py
usage: programmer.py [-h] [--file FILE] [--address ADDRESS_BEGIN]
                     [--address-end ADDRESS_END]
                     serial {write,read,clear}
programmer.py: error: the following arguments are required: serial, mode
```

Required parameters:
- Serial port
- operation mode
	- **write**
	- **read**
	- **clear**

Optional parameters:
- **--address** The start address for the operation
- **--address-end** The final address for the operation
- **--file** Required for **write** and **read** modes, the file to be read/written


### Usage examples

Write the boot.hex file to position 0x0000
```
./programmer.py /dev/porta_serial write --file boot.hex 
```

Write the data.hex in position 0x1E00
```
./programmer.py /dev/porta_serial write --file dado.hex --address 0x1e00
```

Read the entire EEPROM to the dump.bin file
```
./programmer.py /dev/porta_serial read --file dump.bin 
```

Read from EEPROM from the address 0x1E00 to 0x2A00 and create the dump.bin file
```
./programmer.py /dev/porta_serial read --file dump.bin --address 0x1e00 --address-end 0x2a00
```

Clear the entire EEPROM (write 0xFF in all memory locations)
```
./programmer.py /dev/porta_serial clear
```

Clear an EEPROM block
```
./programmer.py /dev/porta_serial clear --address 0x1e00 --address-end 0x2a00
```


## Board Details

Board version 1.3 - 3D view
![3D Board version 1.3](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/PCB_3D_eeprom28_programmer.png)

Board version 1.3
![Board version 1.3](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/PCB_eeprom28_programmer.png)

Board version 1.1 assembled
![Board 1.1 assembled](https://github.com/raltmeyer/28c_programmer/blob/master/28c_programmer_board/img_board_rev1.png)


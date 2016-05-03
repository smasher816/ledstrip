# RGB Ledstrip Controller

This is my current hobby project (name still pending). It is an RGB Ledstrip Controller with many features, and many more planned.

![Controller Image](http://i.imgur.com/3xaks2m.jpg)

For the hardware schematics please see [Smasher816/ledstrip_pcb](https://github.com/smasher816/ledstrip_pcb)

## Current Features

Currently the V1 pcb has support for:
 - 2 rgb ledstrips (6 individual outputs)
 - Stereo 7 band audio input (2 MSGEQ7 chips)
 - Many visualizer modes (see below)
 - Serial terminal for configuration
 - Persistant EEPROM storage of settings

Current Visualizer Modes:
 - Single color
 - [Cycling colors](https://drive.google.com/file/d/0B5rCXUpK42-ULVJfaE1qQ1Iwemc/view?usp=sharing)
 - [Pulse to music](https://drive.google.com/file/d/0B5rCXUpK42-UNk9tTHkxVzlNM2s/view?usp=sharing)
 - [Blinking](https://drive.google.com/file/d/0B5rCXUpK42-USExNNldxSEFJZTA/view?usp=sharing)

## Future Features

Future Modes:
 - "Breathing" color
 - Pattern

A V2 pcb is in the works and will add support for:
 - Configuration through a graphic menu system (No need for a computer!)
 - Rotary encoder inputs
 - OLED screen
 - Onboard USB to UART converter

Plans for future revisions:
 - Add an RTC (change color based on time of day, turn on lights as a morning alarm clock)
 - Bluetooth audio support
 - Daughter board for more outputs


## Setup

You must have an avr toolchain installed (avr-gcc, avr-libc, avr-binutils).

```shell
git submodule update --init
./flashall.sh
```


## Building

```shell
cd build
make
make upload
```


## Configuration

Configuration can be done by editing settings.h and running	`make eeprom`,
or by using the debug terminal.


## Debugging

You may connect to a CLI by via a USB to serial adapter.
Usually this will show up as:
 - `/dev/ttyUSB*` on linux
 - `/dev/tty.usbserial-*` on mac
 - `COM*` on windows

The configuration is as follows;
 - 115200 baud
 - 8 bits
 - 1 stop bit
 - No parity

Once connected please run `help` to see a list of commands

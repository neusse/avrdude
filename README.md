# AVRDUDE

AVRDUDE - AVR Downloader Uploader - is a program for downloading and uploading
the on-chip memories of Microchip’s [AVR microcontrollers](https://en.wikipedia.org/wiki/AVR_microcontrollers).
It can program the Flash and EEPROM, and where supported by the programming
protocol, it can program fuse and lock bits.
AVRDUDE also supplies a direct instruction mode allowing one to issue any
programming instruction to the AVR chip regardless of whether AVRDUDE
implements that specific feature of a particular chip.

AVRDUDE was originally written in 2003 by Brian S. Dean. Since 2006, AVRDUDE has been maintained by Jörg Wunsch,
with the help of [various contributors](/avrdudes/avrdude/graphs/contributors).

## Getting AVRDUDE for Windows

To get AVRDUDE for Windows, install the latest version from the [Releases](./releases) page:

Alternatively, you may [build AVRDUDE](#building-avrdude-for-windows) yourself from source.

## Getting AVRDUDE for Linux

To install AVRDUDE for Linux, install the package `avrdude` by running the following commands:

```console
sudo apt-get install avrdude
```

Alternatively, you may [build AVRDUDE](#building-avrdude-for-linux) yourself from source.

## Using AVRDUDE

AVRDUDE is a command-line application. Run the command `avrdude` without any arguments for a list of options.

A typical command to program your HEX file into your AVR microcontroller looks like this:

```console
avrdude -c <programmer> -p <part> -U flash:w:<file>:i
```

For instance, to program an **Arduino Uno** connected to the serial port **COM1** with a HEX file called `blink.hex`,
you would run the following command:

```console
avrdude -c arduino -P COM1 -b 115200 -p atmega328p -D -U flash:w:objs/blink.hex:i
```

There are many different programmers and options that may be required for the programming to succeed.
For more information, refer to the [AVRDUDE documentation](#todo).

## Building AVRDUDE for Windows

### Windows Prerequisites

TODO.

### Windows Build Instructions

TODO.

## Building AVRDUDE for Linux

### Linux Prerequisites

To build AVRDUDE for Linux, you need to install the following packages:

```console
sudo apt-get install build-essential git automake libtool flex bison libelf-dev libusb-dev libftdi1-dev libhidapi-dev
```

To build the documentation, you need to install the following packages:

```console
sudo apt-get install texlive texi2html
```

### Linux Build Instructions

To build AVRDUDE for Linux, run the following commands:

```console
git clone https://github.com/avrdudes/avrdude
cd avrdude
./bootstrap
./configure
make
```

To build the documentation for AVRDUDE, run the following commands:

```console
cd doc
make all
```

## License

AVRDUDE is licensed under the [GNU GPLv2](./COPYING).

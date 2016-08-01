# nanoTracer
nanoTracer is a basic curve tracer for vacuum tubes. It uses an Arduino for its brain and a simple analog circuit provides measurements command and amplification.

## Dependencies
In order to build this software, you need the following packages :

- GNU make
- avr-gcc
- avr-libc
- avr-binutils
- avrdude

These can be obtained by typing the following command on Debian or Ubuntu systems :

`sudo apt-get install avr-gcc avr-libc avr-binutils avrdude`

## Building nanoTracer
Simply `cd` into the `nanoTracer` directory and use the `make` command.

## Uploading nanoTracer to your device
nanoTracer is compatible with Arduino boards. Once the software has been built, simply type `make upload` to write the software to your device.

When using the AVR Dragon programmer, you need to modify a few variables in the Makefile :

- `PROGRAMMER	= dragon_isp`
- `PROG_PORT	= usb`

## Usage
The Arduino needs to be connected to a computer through USB. The use of a serial terminal, such as `minicom` or `Putty` is required, with a baudrate of 500kbps.

With minicom, use the command `minicom -D /dev/ttyACM0 -b 500000 -o -w`, and make sure to disable software and hardware flow control.

Once the Arduino is ready, it should ouput the string `Starting...`. You can then select the tube type you want to measure by typing its number in the serial terminal window :

- `1` for an ECC81
- `2` for an ECC82
- `3` for an ECC83
- `8` for an ECC88
- `9` for an ECC99

The measure is started by typing `#` in the serial terminal or pressing the hardware button connected to pin D2. The Arduino then outputs a list of measures of the form :

```
Vg = 20
57,1
76,1
92,2
108,5
...
```

Each `Vg = ` line indicates the grid voltage used for this measurement. It is expressed in tenths of volts, for instance, `Vg = 20` means the grid voltage is -2V.

The following lines are the measurements in CSV format, the first value being the anode voltage in V, and the second value being the anode current in tenths of milliamperes. Thus, the line `92,2` means the tube was drawing 0.2mA when the anode voltage was 92V (and the grid voltage -2V). You can then copy/paste these lines in your favorite spreadsheet to exploit the measurements (see the example LibreOffice file in the `spreadsheet/` directory).

## Harware
nanoTracer is a DIY project. You will find all the information needed to build your own tracer under the `kicad/` directory.

## License
Source code for the software is distributed under the GNU GPL v3 license. Please see the COPYING.software file or [the GNU website](http://www.gnu.org/licenses/#GPL) for details.

Hardware design files are distributed under the CERN Open Hardware Licence v1.2. Please see the COPYing.hardware file or [the Open Hardware Repository website](http://www.ohwr.org/licenses/cern-ohl/v1.2) for details.

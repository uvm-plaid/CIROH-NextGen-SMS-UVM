# Setup

Instructions on setting up the Giga with Platform IO and the Swarm M138 modem.
Instructions developed on a 2022 Macbook Air. Your mileage may vary depending
on system being used to set it up.

Author: Jordan Bourdeau

| Revision | Date       | Author          |
|----------|------------|-----------------|
|    1     |   6/16/24  | Jordan Bourdeau |

## Platform IO Configuration

The Giga is not natively supported by Platform IO currently, so some additional
setup needed to be done beyond just picking the board with Platform IO. [This](https://github.com/Haschtl/pio-arduinogiga) GitHub repo was used as a base to port this existing Arduino code to work with Platform IO.
Follow the instructions in the README (editing platform.py file, and using arduino-cli to upload .bin file created by Platform IO). 

NOTE: I had to use `arduino-cli upload -b arduino:mbed_giga:giga -p /dev/cu.usbmodem1301 -i .pio/build/giga/firmware.bin` instead of just `arduino-cli upload -b arduino:mbed_giga:giga -i .pio/build/giga/firmware.bin` during setup process.

With the firmware installed on the Arduino, it will now wait until it is attempted to connect to with a serial device. You can connect using the following command on Platform IO with `pio device monitor`. This will use the port specified in the 'platform.ini' file. You will need to update the port accordingly to whatever the port is on your device (e.g., Wouldlikely be a specified COMM port on Windows).

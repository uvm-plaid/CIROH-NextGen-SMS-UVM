# Shell script created for use on MacOS to automatically build the program,
# find/upload it to a connected Giga, and then monitor it

# Get device path and replace it in platformio.ini
device_port=$(ls /dev/cu.usbmodem*)

if [ -z $device_port ]; then
    echo Could not find connected Giga device.
    exit 1
else
    echo Found device in $device_port
fi

# Build
if pio run ; then
    # Upload to device and run
    arduino-cli upload -b arduino:mbed_giga:giga -p $device_port -i .pio/build/giga/firmware.bin 
    pio device monitor -p $device_port
fi
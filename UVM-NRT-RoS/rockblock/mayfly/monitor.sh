# Shell script created for use on MacOS to automatically build the program,
# find/upload it to a connected Giga, and then monitor it

# Get device path and replace it in platformio.ini
device_port=$(ls /dev/cu.usbserial*)

if [ -z $device_port ]; then
    echo Could not find connected Mayfly.
    exit 1
else
    echo Found device in $device_port
fi

pio device monitor -p $device_port

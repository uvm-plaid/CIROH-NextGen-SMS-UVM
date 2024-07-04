# Get device path and run
device_port=$(ls /dev/cu.usbmodem*)
pio device monitor -p $device_port

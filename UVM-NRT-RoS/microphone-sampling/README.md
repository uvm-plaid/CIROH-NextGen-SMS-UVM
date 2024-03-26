## Workflow for sampling data:
1. Open ArduinoIDE with Mayfly Connected
2. Make sure the TMRpcm library is installed
3. Open up RecordingwTMRpcm.ino file, and make sure the filename, iterations, sample length, and sample rate match what you want. These are all set in the "r" case of the loop function. 
4. Upload sketch
5. Make sure your serial is set to 57600 baud
6. Once you see the message "SD OK" on the serial monitor, send "r" to the board via the serial monitor to start recording.
7. Record sound.
8. Script will stop recording and being writing to the SD card. I try and wait 5 seconds to let it finish writing everything. 
9. Turn Mayfly off, eject the SD card, and then connect it to your computer.
10. You should see the folder TestingSamples on the SD card, and your audio file is within.
11. Move your audio file to your computer.
12. If you are getting bad samples, there are a few ways to attempt to fix this.
  * Change the resistor, which will increase or decrease the gain of the circuit. Higher resistor = more sensitive. (i.e. 47k resistor will make the mic 47x more sensitive than a 1k resistor) Reference [here](https://www.circuitbasics.com/how-to-use-microphones-on-the-arduino/) for a great tutorial. Image of the wiring is also included [here](MicrophoneWiring.png).
  * Change the sampling rate. 44100 is another common value for audio sampling.
  *  Modify the buffer size in /Users/<your-username>/Documents/Arduino/libraries/TMRpcm/pcmConfig.h on line 29. Has to be an even number, default is 128.

# Wiring Notes:
1. There is a switched 5V pin on the mayfly, use that to power the circuit. It's titled "Sv5" on the board, next to all of the analog pins.
2. I have been using a 100k resistor for maximum sensitivity. If it's too sensitive, change this to a lower value resistor.
3. Make sure you connect the right microphone pins. The ground pin is the one with 3 metal lines coming off of the pin. 

## Comments on Future Modifications to Setup
1. Increase the length of the wires connecting the breadboard to the microphone. Given the current setup, the mayfly and the computer are too close to the actual microphone. If sampling when it's snowing/raining, it makes it difficult to avoid the mayfly and computer getting wet. I would recommend finding roughly a 10-foot-long jumper wire if possible, potentially with some sort of weatherized casing.
2. The microphone doesn't really stay in the pin holes of the jumper wires. Explore making this connection stronger, potentially through  soldering them together.
3. Find a better way to encapsulate the mayfly and breadboard in order to protect it from the elements, and secure them in place. Currently is in a plastic takeout container with holes cut out to plug the USB-c in, and the mayfly and breadboard are duct-taped together in it.
4. For the cone + pole setup, wind can cause the cone to swing around and bump into the microphone. Would be great to secure it in place somehow without affecting the sound. 


## Workflow for sampling data:
1. Open ArduinoIDE with Mayfly Connected
2. Make sure the TMRpcm library is installed
3. Open up RecordingwTMRpcm.ino file, and on lines 58 and 63 update file name to match what you'd like.
4. Upload sketch
5. Make sure your serial is set to 57600 baud
6. Once you see the message "SD OK" on the serial monitor, send "r" to the board via the serial monitor to start recording.
7. Record sound.
8. Send "s" to stop the recording, and wait 5 or so seconds for it to finish writing the file. If it's been running for a while, wait longer just in case.
9. Turn Mayfly off, eject the SD card, and then connect it to your computer.
10. You should see the folder TestingSamples on the SD card, and your audio file is within.
11. Move your audio file to your computer.
12. Open the jupyter notebook file "MFCC Extraction.ipynb".
13. Follow the instructions within it. A few things to note:
  * Make sure the sample rate matches what you chose when recording.
  * Completing the noise filtering step is key. The microphone is terrible so the initial sample is pretty bad.
14. If you are getting bad samples, there are a few ways to attempt to fix this.
  * Change the resistor, which will increase or decrease the gain of the circuit. Higher resistor = more sensitive. (i.e. 47k resistor will make the mic 47x more sensitive than a 1k resistor) Reference [here](https://www.circuitbasics.com/how-to-use-microphones-on-the-arduino/) for a great tutorial. Image of the wiring is also included [here](UVM-NRT-RoS/microphone-sampling/MicrophoneWiring.png).
  * Change the sampling rate. 44100 is another common value for audio sampling.
  *  Modify the buffer size in /Users/<your-username>/Documents/Arduino/libraries/TMRpcm/pcmConfig.h on line 29. Has to be an even number, default is 128.

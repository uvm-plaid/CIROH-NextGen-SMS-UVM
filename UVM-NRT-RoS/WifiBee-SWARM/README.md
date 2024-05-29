### TODOS:
- Improve the code that works with the Giga to connect to the SWARM.
- Try and Get WiFi Shield with Mayfly to work?

### Working with the Giga and GigaWifi.ino:
1. Make sure you download both the .ino file and the arduino_secrets.h file, which contains the wifi information for the swarm.
2. Upload the .ino file to the Giga and turn on the SWARM.
3. Wait until you see the Serial Monitor (connect at 9600 baud) output the successful connection messages.
4. Once you're connected, you can read the status messages from the SWARM by sending 'r' over serial to the Giga.
5. Wait for the "$M138 DATETIME*56" message. You may have to wait and send 'r' several times before you see it.
6. Now you can send "Hello World" to the SWARM by sending 's' over serial.
7. Send 'r' until you get a "$TD OK ..." message back, which means the message was accepted.
   
### SWARM:
- Website for Tracking Sattelite Passes: https://kube.tools.swarm.space/pass-checker/
- HIVE Database: https://bumblebee.hive.swarm.space/
  - Hive credentials (uname/passwd/email): wintersmind/UVM4ever/ceskalka@uvm.edu
- Device documentation: https://swarm.space/documentation-swarm/
- ReST API documentation: https://swarm.space/wp-content/uploads/2022/01/Swarm-Hive-API-Integration-Guide.pdf

### Mayfly Notes:
- To get the wifi status lights to work, you need to have SJ17 and SJ16 soldered.
- If we want to have constant power supplied to the Wi-FiBee module, we should also have SJ18 soldered.
- Will need to pass all commands to the mayfly

### Wifi Bee:
- Suggested library by Mayfly makers to use with WiFi Bee: https://github.com/EnviroDIY/TinyGSM/blob/master/README.md
- Wiki for setting up Wifi Bee: https://wiki.dfrobot.com/SKU_TEL0092_WiFi_Bee-ESP8266_Wirelss_module
  - Link to the product: https://www.dfrobot.com/product-1279.html

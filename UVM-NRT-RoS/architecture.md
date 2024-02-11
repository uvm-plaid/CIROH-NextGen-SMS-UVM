Sensor Stations
---------------

LoRa module:
- Duplex serial communication with Mayfly
  + Wire library 
  + Wired I2C serial connection
- LoRa communication 
  + LoraMesh library
  + Relay upstream messages (non-Root nodes)
  + Forward sensor samples (non-Root nodes)

MayFly module:
- Sensors and associated libraries
  + Sensirion RH, Temp, and dewpoint at minimum
- Duplex serial communication with Lora module
  + Wire library
  + Wired I2C serial connection
- Flash memory read/write and associated library
  + (Circular) log sampled data. 

Root Sensor Station
-------------------

Subsumes all hardware/functionality of Sensor Stations. On Root node the LoRa module
forwards all network sensor data to Root Mayfly, which forwards to physically colocated
Swarm via wifi. 

Root MayFly module:
- Bee wifi shield (ESP8266)
- WiFiEsp library
- NMEA parsing and formatting 

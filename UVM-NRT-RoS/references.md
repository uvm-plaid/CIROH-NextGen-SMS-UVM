## Related work

# Audio data for RoS detection

tutorial: https://towardsdatascience.com/extract-features-of-music-75a3f9bc265d

 https://core.ac.uk/download/pdf/33499363.pdf

 https://www.matec-conferences.org/articles/matecconf/pdf/2018/60/matecconf_ici2017_03003.pdf 

 https://www.mdpi.com/2078-2489/11/4/183 

 https://www.researchgate.net/publication/337793617_A_Convolutional_Neural_Networks_Approach_to_Audio_Classification_for_Rainfall_Estimation

 OR

 https://ieeexplore.ieee.org/document/8924399

# General RoS context (from Keith Jennings)

Disdrometers for rain-snow partitioning: https://agupubs.onlinelibrary.wiley.com/doi/full/10.1002/2016JD025387, https://journals.ametsoc.org/view/journals/apme/45/10/jam2406.1.xml

Citizen science obs. of precipitation phase: https://doi.org/10.3389/feart.2021.617594, https://doi.org/10.1029/2022EA002714

Wireless sensor network study in American River basin: https://ieeexplore.ieee.org/abstract/document/8977506, https://agupubs.onlinelibrary.wiley.com/doi/full/10.1002/2016WR019619, https://agupubs.onlinelibrary.wiley.com/doi/full/10.1002/2016WR018825

Hemispheric scale rain-snow partitioning: https://www.nature.com/articles/s41467-018-03629-7

23 unsolved problems in hydrology: https://www.tandfonline.com/doi/full/10.1080/02626667.2019.1620507

Local Climatological Data network, which includes hourly (and sometimes sub-hourly) observations of precipitation phase: https://www.ncei.noaa.gov/maps/lcd/

# Precipitation modeling and sensors

Precipation simulator (great reference for dew point/temp effects): https://scijinks.gov/precipitation-type/

# LoRa 

Mesh Networking: https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=9930341

# WiFi 

The Mayfly is configured for a Bee, aka ESP8266, wifi module. We can probably use this library on the Mayfly for wifi, the client.connect function will allow communication over port 23 on the swarm. 

  https://www.arduino.cc/reference/en/libraries/wifiesp/

A simple web client example provides a reasonable template for how we can connect to Swarm:

  https://github.com/bportaluri/WiFiEsp/blob/master/examples/WebClient/WebClient.ino
 
It is based on this libary but a note on the documentation says this one is retired:

  https://www.arduino.cc/reference/en/libraries/wifi/

# Swarm

We have the evaluator kit with an integrated Swarm M138 module.

Device documentation: https://swarm.space/documentation-swarm/

ReST API documentation: https://swarm.space/wp-content/uploads/2022/01/Swarm-Hive-API-Integration-Guide.pdf


Documentation for the ESP8266 WIFI Bee Shield: https://github.com/esp8266/Arduino?tab=readme-ov-file#installing-with-boards-manager

Tutorial for setting up Wifi Bee: [https://tttapa.github.io/ESP8266/Chap03%20-%20Software.html](https://arduino-esp8266.readthedocs.io/en/latest/installing.html#boards-manager)

I think we can use this library to communicate to the wifi bee from the mayfly: https://github.com/EnviroDIY/TinyGSM/blob/master/README.md

Need to Buy: https://www.dfrobot.com/index.php?route=product/product&product_id=72&search=FTDI&description=true#.Vp3NdVlPrzY

Notes:
- We need to have the power led on, as well as the network connections. Need to have SJ17 and SJ16 soldered.
- If we want to have constant power supplied to the Wi-FiBee module, we should also have SJ18 soldered. 
- [Need to set it up to receive commands through serial from the mayfly]([url](https://wiki.dfrobot.com/SKU_TEL0092_WiFi_Bee-ESP8266_Wirelss_module#target_5))

/**
 * main.cpp
 * 
 * UVM CIROH Research
 * 
 * Main file for running the program operating on the Arduino Giga device
 * which connects to the Swarm via Wifi.
 * 
 * Author: Lars Jensen, Jordan Bourdeau
 */
/**/
#include "credentials.h" 
#include "packets.h"
#include "printing.h"
#include "swarm.h"

#include <ArduinoJson.h>
#include <cstring>
#include <SPI.h>
#include <stdint.h>
#include <time.h>
#include <WiFi.h>

swarm::Device device;
JsonDocument doc;
packets::NodePacket packet;
uint8_t status;
IPAddress server(192, 168, 4, 1);
uint16_t port = 23;

void registerHandlers() {
    packets::registerReceivePacketHandler(device.getPacketHandler());
}

void setup() {
    Serial.begin(9600);
    while (!Serial);
    delay(500);
    device.connect(server, port);

    device.send("RT", "5");
    // Won't get a response if there have been no datetime messages
    device.send("DT", "@", false);
    device.send("FV", "");
}

void loop() {
    char *response = nullptr;
    if (Serial.available()) {                
        switch(Serial.read()) {
            case 'g':
                if ((response = device.readMessage()) != nullptr) {
                    printing::dbgln("%s", response);
                } else {
                    printing::dbgln("No message yet");
                }
                break;   
            case 'd':
                printing::dbgln("Disconnecting successful: %d", device.disconnect());
                break;
            case 'r':
                printing::dbgln("Reconnecting successful: %d", device.connect(server, port));
                break;
            case 'c':
                printing::dbgln("Client connected: %d, available: %d", device.connected(), device.available());
                break;
        }
    }

    if (!device.connected()) {
        device.disconnect();
    }

}


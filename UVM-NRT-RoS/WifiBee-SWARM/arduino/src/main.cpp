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

// Helper function for debugging if printing is having any issues
void loopLeds() {
    digitalWrite(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}

void registerHandlers() {
    packets::registerReceivePacketHandler(device.getPacketHandler());
}


void setup() {
    Serial.begin(9600);
    while (!Serial);
    delay(500);
    device.connect(server, port);

    const char *commands[][2] = {
        {"FV", ""}
    };
    for (auto cmd: commands) {
        swarm::Command command = swarm::Command(cmd[0], cmd[1]);
        swarm::CommandResponse response = device.sendCommand(command);
        printing::dbgln("Sent $%s, Status: %d, Response: %s", command.data, response.status, response.data);
    }
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


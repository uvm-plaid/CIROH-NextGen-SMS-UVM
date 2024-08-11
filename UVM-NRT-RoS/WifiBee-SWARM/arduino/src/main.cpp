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
/*#include "credentials.h" */
#include "packets.h"
#include "printing.h"
#include "swarm.h"
/**/
/*#include <ArduinoJson.h>*/
/*#include <cstring>*/
/*#include <SPI.h>*/
/*#include <stdint.h>*/
/*#include <time.h>*/
/*#include <WiFi.h>*/
/**/
swarm::Device device;
JsonDocument doc;
packets::NodePacket packet;
/**/
/*WiFiClient client;*/
/*uint8_t status;*/
/*IPAddress server(192, 168, 4, 1);*/
/*uint16_t port = 23;*/
/**/
/*// Helper function for debugging if printing is having any issues*/
/*void loopLeds() {*/
/*    digitalWrite(LED_BUILTIN, OUTPUT);*/
/*    digitalWrite(LED_BUILTIN, LOW);*/
/*    digitalWrite(LED_BUILTIN, HIGH);*/
/*    delay(1000);*/
/*    digitalWrite(LED_BUILTIN, LOW);*/
/*    delay(1000);*/
/*}*/
/**/
/*void registerHandlers() {*/
/*    packets::registerReceivePacketHandler(swarmClient.getPacketHandler());*/
/*}*/

// Created by Lars Jensen 2024
// For UVM Research 

#include <SPI.h>
#include <WiFi.h>

#define SECRET_SSID "swarm-1dac"
#define SECRET_PASS "12345678"

// ------------------------------------------------------------------------------
// Please enter your sensitive data in the Secret tab/arduino_secrets.h
// ------------------------------------------------------------------------------

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status

// ------------------------------------------------------------------------------
// To connect to the Access Point on the SWARM (how you actually upload the data)
// ------------------------------------------------------------------------------

WiFiClient client;
IPAddress server(192,168,4,1);
int port = 23;

// --------------------------------------------------------------------------------
// Read in all data from the client (SWARM access point)
// --------------------------------------------------------------------------------

void readContinously(){
    Serial.print("Client available: ");
    Serial.println(client.available());
    while (client.available()) {

        char c = client.read();
        if (c == '$') {
            Serial.println();
        }
        Serial.print(c);
    }
}

// --------------------------------------------------------------------------------
// WiFi Methods taken from Arduino Examples
// --------------------------------------------------------------------------------


void setup() {

    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    // ------------------------------------------------------------------------------
    // Initialize serial and wait for port to open:
    // ------------------------------------------------------------------------------

    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    delay(1000);

    device.connect(server, port);
    client = device.client;

}

void loop() {

    int index = 0;
    char helloWorld[23] = "$TD \"Hello World!\"*31\n";

    // --------------------------------------------------------------------------------
    // Read in commands from serial to either read data coming in or send out message
    // --------------------------------------------------------------------------------

    if(Serial.available()){                
        switch(Serial.read()){
            case 's':
                Serial.println("Sending Hello World");
                client.flush();
                while (helloWorld[index]) {
                    client.print(helloWorld[index]);
                    index++;
                }
                Serial.println("Hello World Message Sent");
                index = 0;
                break; 
            case 'r':
                Serial.println("Trying to read!");
                readContinously();
                break;   
            case 'd':
                printing::dbgln("Disconnecting successful: %d", device.disconnect());
                break;
            case 'c':
                printing::dbgln("Reconnecting successful: %d", device.reconnect());
                break;
        }
    }

    // --------------------------------------------------------------------------------
    // If we've lost the connection, disconnect from the client
    // --------------------------------------------------------------------------------

    if (!client.connected()) {
        device.disconnect();
    }

}


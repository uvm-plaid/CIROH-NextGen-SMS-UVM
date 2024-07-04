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

swarm::Device client;
JsonDocument doc;
JsonDocument doc2;
packets::NodePacket packet;

void setup() {
    // Register handlers
    packets::registerReceivePacketHandler(client.getPacketHandler());

    // Wait for serial port to connect. Needed for native USB port only
    Serial.begin(9600);
    while (!Serial) {
      ;
    }

    // To connect to the Access Point on the SWARM (how you actually upload the data)
    IPAddress server(192, 168, 4, 1);
    int port = 23;
    client.connect(server, port);

    doc["name"] = "Test";
    doc["temperature"] = 72.0;
}

void loop() {

    // Keep sending packets to the client every minute
    packet = packets::makeNodePacket(doc);
    packets::receivePacket(packet);
    delay(5 * 1000);

    // If we've lost the connection, disconnect from the client
    if (!client.isConnected()) {
        printing::dbgln("Disconnecting from server.");
        client.disconnect();

        // Do nothing forevermore:
        while (true);
    }
}

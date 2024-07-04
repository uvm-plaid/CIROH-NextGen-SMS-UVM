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
packets::NodePacket packet;

// Helper function for debugging if printing is having any issues
void loopLeds() {
    digitalWrite(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}

void setup() {
    // Register handlers
    packets::registerReceivePacketHandler(client.getPacketHandler());

    // Wait for serial port to connect. Needed for native USB port only
    Serial.begin(9600);
    while (!Serial) {
        loopLeds();
    }

    printing::dbgln("Serial port is connected!");

    // To connect to the Access Point on the SWARM (how you actually upload the data)
    IPAddress server(192, 168, 4, 1);
    int port = 23;
    client.connect(server, port);

    // Read any buffer data
    client.readData();

    // Send startup commands
    const char *commands[][2] = {
        {"RT", "?"},
        {"FV", ""},
        {"MT", "C=U"}
    };

    // TODO: Send setup commands here- segfaulting currently
    for (auto command: commands) {
        char *response = client.sendCommand(swarm::Command(command[0], command[1]));
        printing::dbgln("Response: %s", response);
    }

    // Setup JSON packet data to be sent in a loop
    doc["name"] = "Test";
    doc["temperature"] = 72.0;
}

// NOTE: Very first command after connecting can take a while
void loop() {

    // Keep sending packets to the client
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

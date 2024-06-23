/**
 * modem.cpp
 * 
 * File containing implementation for modem functions.
 * 
 * Authors: Lars Jensen, Jordan Bourdeau
 * Date Created: 6/23/24
*/

#include "checksum.h"
#include "swarm.h"

#include <SPI.h>
#include <WiFi.h>

void sendCommand(WiFiClient client, Command command) {
    uint8_t checksum = nmeaChecksum(command.command, command.length);
    char checksum_buffer[CHECKSUM_LENGTH];
    sprintf(checksum_buffer, "*%02x", checksum);

    client.flush();
    sendString(client, "$", 1);
    sendString(client, command.command, command.length);
    sendString(client, checksum_buffer, CHECKSUM_LENGTH);

    // Logging
    Serial.print("Sending command: ");
    Serial.print('$');
    Serial.print(command.command);
    Serial.print(checksum_buffer);
    Serial.println();

    // Try to read response
    char *response = readData(client);
    Serial.print("Response: ");
    Serial.println(response);
}

void sendString(WiFiClient client, char *data, uint32_t length) {
    int index = 0;
    while (index < length) {
        client.print(data[index++]);
    }
}

char *readData(WiFiClient client) {
    static char message[MAX_MESSAGE_LENGTH];
    int index = 0; 

    if (client.available()) {
        // Read until we get to the start of a message
        char c = client.read();
        while (c != '$') {
            c = client.read();
        }

        // Read the incoming message into the message array
        while (c != '$' && index < MAX_MESSAGE_LENGTH - 1) { 
            message[index++] = c; 
            c = client.read(); 
        }

        // Add null terminator to mark the end of the string
        message[index] = '\0';
    }

    return message;
}

void readContinuously(WiFiClient client){
    while (client.available()) {
        char c = client.read();
        if (c == '$') {
            Serial.println();
        }
        Serial.print(c);
    }
}

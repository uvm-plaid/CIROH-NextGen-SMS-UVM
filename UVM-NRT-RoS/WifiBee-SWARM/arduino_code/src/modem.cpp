/**
 * modem.cpp
 * 
 * File containing implementation for modem functions.
 * 
 * Authors: Lars Jensen, Jordan Bourdeau
 * Date Created: 6/23/24
*/

#include "modem.h"

#include <SPI.h>
#include <WiFi.h>

void flushRssi(WiFiClient client) {
    Serial.println("Flushing RSSI messages.");
    char cmd[9] = "$RT 0*16";

    client.flush();
    int index = 0;
    while (cmd[index]) {
        client.print(cmd[index++]);
    }
}

char* readData(WiFiClient client) {
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

    Serial.println(message);
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


uint8_t nmeaChecksum (const char *sz, size_t len) {
    size_t i = 0;
    uint8_t cs;

    if (sz [0] == '$'){
        i++;
    }

    for (cs = 0; (i < len) && sz [i]; i++){
        cs ^= ((uint8_t) sz [i]);
    }

    return cs;
}

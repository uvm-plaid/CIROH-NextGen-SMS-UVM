/**
 * printing.cpp
 * 
 * File containing implementations for debugging print functions.
 * Mostly gathered from Arduino WiFi examples.
 * 
 * Source Information:
 *  Circuit:
 *  GIGA R1 WiFi
 * 
 *  Created 13 July 2010 by dlf (Metodo2 srl)
 *  Modified 31 May 2012 by Tom Igoe
 *  Modified 22 March 2023 by Karl Söderby
 * 
 * Author: Lars Jensen
 * Date Created: 6/23/24
*/

#include "printing.h"

#include <SPI.h>
#include <WiFi.h>

namespace printing {

    void printMacAddress(uint8_t mac[]) {
        for (int i = 5; i >= 0; i--) {
            if (mac[i] < 16) {
                Serial.print("0");
            }
            Serial.print(mac[i], HEX);
            if (i > 0) {
                Serial.print(":");
            }
        }
        Serial.println();
    }

    void printWifiData() {
        // Print your board's IP address:
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");
        Serial.println(ip);
        Serial.println(ip);

        // Print your MAC address:
        byte mac[6];
        WiFi.macAddress(mac);
        Serial.print("MAC address: ");
        printMacAddress(mac);
    }

    void printCurrentNet() {
        // Print the SSID of the network you're attached to:
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        // Print the MAC address of the router you're attached to:
        byte bssid[6];
        WiFi.BSSID(bssid);
        Serial.print("BSSID: ");
        printMacAddress(bssid);

        // Print the received signal strength:
        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.println(rssi);

        // Print the encryption type:
        byte encryption = WiFi.encryptionType();
        Serial.print("Encryption Type:");
        Serial.println(encryption, HEX);
        Serial.println();
    }
}

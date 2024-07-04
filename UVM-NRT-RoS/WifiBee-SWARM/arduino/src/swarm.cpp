/**
 * modem.cpp
 * 
 * File containing implementation for modem functions.
 * 
 * Authors: Lars Jensen, Jordan Bourdeau
 * Date Created: 6/23/24
*/

#include "checksum.h"
#include "credentials.h"
#include "printing.h"
#include "swarm.h"

#include <SPI.h>
#include <WiFi.h>

namespace swarm {

    Command::Command(const char code[COMMAND_CODE_LENGTH + 1], packets::NodePacket &packet) {
        // Write command code to buffer
        data[0] = code[0];
        data[1] = code[1];
        data[2] = ' ';

        // Write packet data to buffer
        for (size_t i = 0; i < packet.length; ++i) {
            data[i + 3] = packet.data[i];
        }

        // Compute checksum and write it (plus null-terminating character) to data buffer
        length = COMMAND_CODE_LENGTH + 1 + packet.length;
        uint8_t checksum = checksum::nmeaChecksum(data, length);
        snprintf((char *) (data + length), CHECKSUM_LENGTH + 1, "*%02x", checksum);

        // Update length (length is length of legitimate bytes, and does not include null character).
        length += CHECKSUM_LENGTH;
    }

    Device::Device() : status(WL_IDLE_STATUS) {}

    Device::~Device() {
        disconnect(); 
    }

    bool Device::isConnected() {
        return client.connected();
    }

    bool Device::connect(IPAddress server, uint16_t port, uint8_t retries, float connectDelay) {
        uint8_t timesAttempted = 0;

        #define CHECK_RETRIES(timesAttempted, retries) {\
            if (timesAttempted - 1 > retries) {\
                return false;\
            } else {\
                timesAttempted = 0;\
            }\
        }\

        // Check for the WiFi module:
        if (WiFi.status() == WL_NO_MODULE) {
            printing::dbg("Communication with WiFi module failed!");
            return false;
        }

        // Attempt to connect to WiFi network:
        while (++timesAttempted <= retries && status != WL_CONNECTED) {
            printing::dbg("Attempting to connect to WPA SSID: ");
            printing::dbgln(credentials::SWARM_SSID);

            // Connect to WPA/WPA2 network:
            status = WiFi.begin(credentials::SWARM_SSID, credentials::SWARM_PASSWORD);

            // Wait for connection
            delay(connectDelay);
        }
        
        CHECK_RETRIES(timesAttempted, retries);

        // You're connected now, so print out the data:
        printing::dbg("You're connected to the network");
        printing::printCurrentNet();
        printing::printWifiData();

        // Now we'll try and open a connection to the Access Point on the wifi to the swarm
        printing::dbgln("\nStarting Access Point Connection...");
        while (++timesAttempted <= retries && !client.connect(server, port)) {
            printing::dbgln("Trying to connect");
            delay(connectDelay);
        }

        CHECK_RETRIES(timesAttempted, retries);

        printing::dbgln("Connected!");

        return true;
    }

    bool Device::disconnect() {
        client.stop();
        return true;
    }

    packets::PacketHandler Device::getPacketHandler() {
        return [this](packets::NodePacket handler) {
            this->receivePacket(handler);
        };
    }

    void Device::receivePacket(packets::NodePacket packet) {
        sendCommand(Command("TD", packet));
    }

    void Device::sendCommand(Command command) {
        printing::dbgln("Sending command: $%s", (char *) command.data);

        client.flush();
        client.print('$');
        sendData(command.data, command.length);
        printing::dbgln("Command sent!");
    }

    void Device::sendData(uint8_t data[], size_t length) {
        uint32_t index = 0;
        while (index < length) {
            client.print(data[index++]);
        }
    }

    uint8_t* Device::readData() {
        static uint8_t message[MAX_NODE_PACKET_SIZE_BYTES + 1];
        int index = 0; 

        if (client.available()) {
            // Read until we get to the start of a message
            char c = client.read();
            while (c != '$') {
                c = client.read();
            }

            // Read the incoming message into the message array
            while (c != '$' && index < MAX_NODE_PACKET_SIZE_BYTES) { 
                message[index++] = c; 
                c = client.read(); 
            }

            // Add null terminator to mark the end of the string
            message[index] = '\0';
        }

        return message;
    }

    void Device::readContinuously() {
        while (client.available()) {
            char c = client.read();
            if (c == '$') {
                printing::dbgln("");
            }
            printing::dbg("%c", c);
        }
    }
}


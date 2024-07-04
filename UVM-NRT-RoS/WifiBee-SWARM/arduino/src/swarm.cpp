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

    #define CHECK_VALID_COMMAND(code, data) {\
        size_t length;\
        if ((length = strlen(code)) > COMMAND_CODE_LENGTH) {\
            printing::dbgln("Command code was %d characters long but can only be %d characters.", length, MAX_NODE_PACKET_SIZE_BYTES);\
            return;\
        }\
        if ((length = strlen((char *) data)) > MAX_NODE_PACKET_SIZE_BYTES) {\
            printing::dbgln("Data was %d characters long but can only be %d characters.", length, MAX_NODE_PACKET_SIZE_BYTES);\
            return;\
        }\
    }\

    Command::Command(const char code[COMMAND_CODE_LENGTH + 1], packets::NodePacket packet) {
        CHECK_VALID_COMMAND(code, packet.data);

        // Write command code to buffer
        data[0] = code[0];
        data[1] = code[1];
        data[2] = ' ';

        // Write packet data to buffer (+1 for null byte)
        strncpy((char *) data + 3, (char *) packet.data, packet.length + 1);

        // Compute checksum and write it (plus null-terminating character) to data buffer
        length = COMMAND_CODE_LENGTH + 1 + packet.length;
        uint8_t checksum = checksum::nmeaChecksum(data, length);
        snprintf((char *) (data + length), CHECKSUM_LENGTH + 1, "*%02x", checksum);

        // Update length (length is length of legitimate bytes, and does not include null character).
        length += CHECKSUM_LENGTH;

        data[length] = '\0';
        length += 1;
    }

    Command::Command(const char code[COMMAND_CODE_LENGTH + 1], const char *data) 
        : Command(code, packets::NodePacket(data)) {}

    void Command::print() {
        printing::dbgln("Length: %d bytes", length);
        printing::dbgln("Data: %s", data);
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

        // Wait for the "$M138 DATETIME*56" message
        readContinuously();

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


    char *Device::sendCommand(const char code[COMMAND_CODE_LENGTH + 1], const char *data, bool awaitResponse) {
        return sendCommand(Command(code, data), awaitResponse);
    }

    char *Device::sendCommand(Command command, bool awaitResponse) {
        printing::dbgln("Sending command: $%s", (char *) command.data);

        client.flush();
        client.print('$');
        sendData(command.data, command.length);
        printing::dbgln("Command sent!");

        // TODO: Remove
        delay(2000);

        return awaitResponse ? readData() : nullptr;
    }

    void Device::sendData(uint8_t data[], size_t length) {
        uint32_t index = 0;
        while (index < length) {
            client.print(data[index++]);
        }
    }

    char* Device::readData() {
        static char message[MAX_NODE_PACKET_SIZE_BYTES + 1];
        int index = 0; 

        if (client.available()) {
            // Read until we get to the start of a message
            char c = client.read();
            while (c != '$') {
                c = client.read();
            }

            // Read the incoming message into the message array
            while (c != '$' && index < 79) { 
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


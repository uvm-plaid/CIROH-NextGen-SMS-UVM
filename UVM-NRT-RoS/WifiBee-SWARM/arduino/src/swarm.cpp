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

    const char *STATUS_MESSAGES[] = {
        "OKAY",
        "NONE",
        "CMD_BADPARAM",
        "CMD_BADPARAMLENGTH",
        "CMD_INVALIDCHAR",
        "CMD_NOTIMPLEMENTED",
        "CMD_PARAMMISSING",
        "CMD_PARAMDUPLICATE"
    };

    Command::Command(const char code[COMMAND_CODE_LENGTH + 1], packets::NodePacket packet) {
        size_t length;
        if ((length = strlen(code)) > COMMAND_CODE_LENGTH) {
            printing::dbgln("Command code was %d characters long but can only be %d characters.", length, MAX_NODE_PACKET_SIZE_BYTES);
            return;
        } else if ((length = strlen(packet.data)) > MAX_NODE_PACKET_SIZE_BYTES) {
            printing::dbgln("Data was %d characters long but can only be %d characters.", length, MAX_NODE_PACKET_SIZE_BYTES);
            return;
        }

        // Write command code to buffer
        for (int i = 0; i < COMMAND_CODE_LENGTH; data[i] = code[i], ++i);
        int prefixLength = 2;
        if (packet.length > 0) {
            data[prefixLength++] = ' ';
        }

        // Write packet data to buffer (+1 for null byte)
        strncpy(data + prefixLength, packet.data, packet.length + 1);

        // Compute checksum and write it (plus null-terminating character) to data buffer
        length = prefixLength + packet.length;
        uint8_t checksum = checksum::nmeaChecksum(data, length);
        snprintf((data + length), CHECKSUM_LENGTH + 1, "*%02x", checksum);

        // Update length (length is length of legitimate bytes, and does not include null character).
        length += CHECKSUM_LENGTH;

        data[length++] = '\0';
    }

    Command::Command(const char code[COMMAND_CODE_LENGTH + 1], const char *data) 
        : Command(code, packets::NodePacket(data)) {}

    void Command::print() {
        printing::dbgln("Length: %d bytes", length);
        printing::dbgln("Data: %s", data);
    }

    CommandResponse::CommandResponse() {}

    CommandResponse::CommandResponse(char response[MAX_MESSAGE_LENGTH + 1]) {
        strncpy(data, response, MAX_MESSAGE_LENGTH + 1);
        status = CommandStatus::NONE;

        // #define ERROR_STR "ERR,"
        // #define ERROR_STR_LEN 4

        // // Get our own copy of the data and keep track of length
        // strncpy(data, response, MAX_MESSAGE_LENGTH + 1);

        // // Check special exit conditions
        // if (data == nullptr) {
        //     status = CommandStatus::NONE;
        //     return;
        // } 
        // char *ret;
        // if ((ret = strstr(data, ERROR_STR)) == nullptr) {
        //     status = CommandStatus::OKAY;
        //     return;
        // }

        // // Compare error string against all potential candidates
        // for (int index = CommandStatus::CMD_BADPARAM; index < CommandStatus::CMD_PARAMDUPLICATE; ++index) {
        //     if (strstr(ret + ERROR_STR_LEN, STATUS_MESSAGES[index]) != nullptr) {
        //         status = (CommandStatus) index;
        //         printing::dbgln(statusMessage());
        //         return;
        //     }
        // }
    }

    const char *CommandResponse::statusMessage() {
        return STATUS_MESSAGES[status];
    }

    Device::Device() : status(WL_IDLE_STATUS) {}

    Device::~Device() {
        disconnect(); 
    }

    bool Device::isConnected() {
        return client.connected();
    }

    bool Device::connect(IPAddress server, uint16_t port, uint32_t connectDelay) {
        if (WiFi.status() == WL_NO_MODULE) {
            printing::dbgln("Communication with WiFi module failed!");
            return false;
        }

        while (status != WL_CONNECTED) {
            printing::dbgln("Attempting to connect to WPA SSID: %s", credentials::SWARM_SSID);
            status = WiFi.begin(credentials::SWARM_SSID, credentials::SWARM_PASSWORD);
            delay(connectDelay);
        }

        printing::dbgln("Connected to the network.");
        printing::printCurrentNet();
        printing::printWifiData();

        printing::dbgln("Starting access point connection...");
        while (!client.connected() && client.connect(server, port)) {
            printing::dbgln("Access point connection failed. Retrying.");
            delay(3000);
        }

        printing::dbgln("Client connected.");
        return true;
    }

    bool Device::disconnect() {
        if (client.connected()) {
            client.stop();
        }
        if (WiFi.status() == WL_CONNECTED) {
            WiFi.disconnect();
        }
        WiFi.end();
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

    char* Device::sendCommand(const char code[COMMAND_CODE_LENGTH + 1], const char *data, bool awaitResponse) {
        return sendCommand(Command(code, data), awaitResponse);
    }

    char* Device::sendCommand(Command command, bool awaitResponse) {
        printing::dbgln("Sending command: $%s", command.data);
        
        // Cannot send the command
        if (!client.available()) {
            return nullptr;
        }

        client.flush();
        client.print('$');

        uint32_t index = 0;
        while (index < command.length) {
            client.print(command.data[index++]);
        }
        
        client.print('\n');

        printing::dbgln("Command sent!");
        char *response = awaitResponse ? readDataBlocking() : nullptr;
        printing::dbgln(response);
        // return CommandResponse(response);
        return response;
    }

    char *Device::readData() {
        static char MESSAGE[MAX_MESSAGE_LENGTH + 1];
        size_t index = 0;
        char c;
        printing::dbgln("Read Data. Is client available? %d", client.available());
        if (!client.available()) {
            return nullptr;
        }

        memset(MESSAGE, 0, MAX_MESSAGE_LENGTH + 1);

        // Read to the start of the message
        while ((c = client.read()) != '$');

        // Read the message into the array unless escaped by $ or hit end of message buffer
        while ((c = client.read()) != '$' && index < MAX_MESSAGE_LENGTH) {
            MESSAGE[index++] = c;
        }

        return MESSAGE;
    }

    char *Device::readDataBlocking() {
        char *message;
        while (true) {
            while ((message = readData()) == nullptr) {
                printing::dbgln("No message :(");
                delay(500);
            }
            printing::dbgln(message);
        }
        return message;
    }
}


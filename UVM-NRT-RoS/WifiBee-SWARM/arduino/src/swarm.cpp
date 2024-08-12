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
        size_t len;
        if ((len = strlen(code)) > COMMAND_CODE_LENGTH) {
            printing::dbgln("Command code was %d characters long but can only be %d characters.", len, MAX_NODE_PACKET_SIZE_BYTES);
            return;
        } else if ((len = strlen(packet.data)) > MAX_NODE_PACKET_SIZE_BYTES) {
            printing::dbgln("Data was %d characters long but can only be %d characters.", len, MAX_NODE_PACKET_SIZE_BYTES);
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

    CommandResponse::CommandResponse(): status(CommandStatus::NONE) {}

    CommandResponse::CommandResponse(char response[MAX_MESSAGE_LENGTH + 1]) {
        constexpr char *error_str = "ERR,";
        constexpr size_t error_str_len = 4;
        if (data == nullptr) {
            status = CommandStatus::NONE;
            return;
        }
        strncpy(data, response, MAX_MESSAGE_LENGTH + 1);
        char *ret;
        if ((ret = strstr(data, error_str)) == nullptr) {
            status = CommandStatus::OKAY;
            return;
        }

        // Compare error string against all potential candidates
        for (int index = CommandStatus::CMD_BADPARAM; index < CommandStatus::CMD_PARAMDUPLICATE; ++index) {
            if (strstr(ret + error_str_len, STATUS_MESSAGES[index]) != nullptr) {
                status = (CommandStatus) index;
                return;
            }
        }
    }

    const char *CommandResponse::statusMessage() {
        return STATUS_MESSAGES[status];
    }

    Device::Device() : status(WL_IDLE_STATUS) {}

    Device::~Device() {
        disconnect(); 
        WiFi.end();
    }

    bool Device::connect(IPAddress server, uint16_t port, uint32_t connectDelay) {
        if (client.connected()) {
            return true;
        }

        this->server = server;
        this->port = port;
        this->connectDelay = connectDelay;

        if (WiFi.status() == WL_NO_MODULE) {
            printing::dbgln("Communication with WiFi module failed!");
            return false;
        }

        while (status != WL_CONNECTED) {
            printing::dbgln("Attempting to connect to WPA SSID: %s", credentials::SWARM_SSID);
            WiFi.disconnect();
            status = WiFi.begin(credentials::SWARM_SSID, credentials::SWARM_PASSWORD);
            delay(connectDelay);
        }

        printing::dbgln("Connected to the network.");
        printing::printCurrentNet();
        printing::printWifiData();

        printing::dbgln("Starting access point connection...");
        while (!client.connected() && client.connect(server, port)) {
            printing::dbgln("Access point connection failed. Retrying.");
            delay(100);
        }

        printing::dbgln("Client connected.");
        return true;
    }

    bool Device::reconnect() {
        if (connected()) {
            return true;
        }
        return connect(server, port);
    }

    bool Device::disconnect() {
        if (connected()) {
            client.stop();
            return WiFi.status() == WL_DISCONNECTED;
        }
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

    CommandResponse Device::sendCommand(const char code[COMMAND_CODE_LENGTH + 1], const char *data, bool awaitResponse) {
        return sendCommand(Command(code, data), awaitResponse);
    }

    CommandResponse Device::sendCommand(Command command, bool awaitResponse) {
        if (!client.connected()) {
            printing::dbgln("Client is not connected.");
            return CommandResponse(nullptr);
        }

        client.write('$');
        client.write(command.data, command.length);
        client.write('\n');
        client.flush();

        char *response = awaitResponse ? readMessageBlocking() : nullptr;
        return CommandResponse(response);
    }

    char *Device::readMessage() {
        static char MESSAGE[MAX_MESSAGE_LENGTH + 1];
        size_t index = 0;
        char c;

        if (!client.available()) {
            return nullptr;
        }

        memset(MESSAGE, 0, MAX_MESSAGE_LENGTH + 1);

        // Read to the start of the message
        while ((c = client.read()) != '$');

        // Read the message into the array unless escaped by $ or hit end of message buffer
        while ((c = client.read()) != '$'
                && c != 0xFF // Padding bytes
                && index < MAX_MESSAGE_LENGTH) {
            MESSAGE[index++] = c;
        }
        MESSAGE[index] = '\0';

        return MESSAGE;
    }

    char *Device::readMessageBlocking() {
        char *message;
        while ((message = readMessage()) == nullptr) {
            delay(500);
        }
        return message;
    }
}


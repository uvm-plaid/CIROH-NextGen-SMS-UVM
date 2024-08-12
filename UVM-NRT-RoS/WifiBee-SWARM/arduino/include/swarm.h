/**
 * swarm.h
 * 
 * Header file containing functions for interacting with the modem device.
 * 
 * Created as part of the UVM CIROH project.
 * 
 * TODO: need to read data until we get "$M138 DATETIME*56" message, once we do we can transmit data
 * TODO: send "$TD ..." message with our data
 * TODO: wait for "$TD Okay message" received to confirm our message was sent
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/16/24 
*/

#ifndef SWARM_H
#define SWARM_H

#include "packets.h"

#include <stdint.h>
#include <WiFi.h>

namespace swarm {
    #define COMMAND_CODE_LENGTH 2 // commands are 2 characters long
    #define CHECKSUM_LENGTH 3 // *XX
    #define MAX_MESSAGE_LENGTH 79
    // + 1 for null byte
    constexpr size_t MAX_COMMAND_LENGTH = MAX_NODE_PACKET_SIZE_BYTES + 1;

    struct Command {
        size_t length;
        char data[MAX_COMMAND_LENGTH];

        /**
         * Constructor used to make a command object from a packet and command code.
         * Automatically computes length and checksum value.
         * `data` field will contain value to send to the Swarm device.
         * 
         * @param code (const char [COMMAND_CODE_LENGTH + 1]): 2 character command code with null-termination.
         *  for the command length.
         * @param packet (NodePacket packet): Packet containing data used in command.
        */
        Command(const char code[COMMAND_CODE_LENGTH + 1], packets::NodePacket packet);

        /**
         * Convenience constructor which wraps the one taking a node packet.
         * @param code (const char [COMMAND_CODE_LENGTH + 1]): 2 character command code with null-termination.
         * @param data (const char *): Null terminated string for the command argument.
        */
        Command(const char code[COMMAND_CODE_LENGTH + 1], const char *data);
    };

    enum CommandStatus {
        OKAY,
        NONE,
        CMD_BADPARAM,
        CMD_BADPARAMLENGTH,
        CMD_BADPARAMVALUE,
        CMD_INVALIDCHAR,
        CMD_NOTIMPLEMENTED,
        CMD_PARAMMISSING,
        CMD_PARAMDUPLICATE,
    };

    extern const char *STATUS_MESSAGES[];

    struct CommandResponse {
        char data[MAX_MESSAGE_LENGTH + 1];
        CommandStatus status;

        /**
         * Default initializer.
        */
        CommandResponse();

        /**
         * Constructor which takes a string response and determines
         * the command status.
        */
        CommandResponse(char response[MAX_MESSAGE_LENGTH + 1]);

        /**
         * @returns String status message for response status.
        */
        const char *statusMessage();
    };

    /**
     * Struct providing the Swarm device interface.
     * @param client (WiFiClient): WiFi connection to Swarm device.
     * @param status (uint8_t): Current status of the client connection.
    */
    struct Device {
        public:
        WiFiClient client;
        uint8_t status;
        IPAddress server;
        uint16_t port;
        uint32_t connectDelay;

        /**
         * Constructor for the Device struct.
        */
        Device();

        /**
         * Destructor for Device which disconnects from client.
        */
        ~Device();

        /**
         * Getter for if the Swarm device is connected.
         * @returns Whether WiFi client is connected.
        */
        bool connected() { return client.connected(); }

        /**
         * Getter for whether the client is available or not.
         * @returns Number of bytes available to be read.
         */
        int available() { return client.available(); }

        /**
         * Function for connecting to the Swarm device.
         * @param server (IPAddress): IP address of WiFi client.
         * @param port (uint16_t): Port to connect to on WiFiClient.
         * @param connectDelay (uint32_t): Number of milliseconds to wait after connecting to
         *  WiFi network and between connection attempts to Swarm access point.
        */
        bool connect(IPAddress server, uint16_t port, uint32_t connectDelay = 500);
        
        /**
         * Reconnect to the last used connection.
         */
        bool reconnect();

        /**
         * Function for disconnecting from the Swarm device.
        */
        bool disconnect();

        /**
         * Function to retrieve the packet handler from a Swarm object.
         * @returns (PacketHandler) Swarm handler for when a packet is received.
        */
        packets::PacketHandler getPacketHandler();

        /**
         * Handler function for the Swarm receiving a packet.
         * @param packet (packets::NodePacket): Packet being received.
        */
        void receivePacket(packets::NodePacket packet);

        /**
         * Convenience function which abstracts the `Command` data structure
         * and presents a string interface.
         * 
         * @param command (const char [COMMAND_CODE_LENGTH + 1]): 2-character command code.
         * @param data (const char *): Data to be sent in command.
         * @param awaitResponse (bool): Whether to wait for a response from the modem.
         * 
         * @returns (CommandResponse): Parsed response.
        */
        CommandResponse sendCommand(const char code[COMMAND_CODE_LENGTH + 1], const char *data, bool awaitResponse = true);

        /**
         * Function responsible for sending a SWARM command.
         * @param command (Command): Object containing relevant information
         *  to issue a command to the SWARM device. Should NOT include the 
         *  leading '$' or trailing checksum.
         * @param awaitResponse (bool): Whether to wait for a response from the modem.
         * 
         * @returns (CommandResponse): Parsed response.
        */
        CommandResponse sendCommand(Command command, bool awaitResponse = true);

        /**
         * Function to read a single message from the client.
         * Reads in data until it encounters a '$' character, then
         * continues reading until it encounters a newline or end of message.
         * 
         * @returns (char *): Pointer to static character array of up to MAX_MESSAGE_LENGTH.
        */
        char *readMessage();

        /**
         * Function which blocks while reading data until a message is received.
         * 
         * @returns (char *): Pointer to static character array of up to MAX_MESSAGE_LENGTH.
        */
        char *readMessageBlocking();
    };
}

#endif /* SWARM_H */

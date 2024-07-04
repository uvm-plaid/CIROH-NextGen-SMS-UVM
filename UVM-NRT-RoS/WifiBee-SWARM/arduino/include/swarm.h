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
    constexpr size_t MAX_COMMAND_LENGTH = MAX_NODE_PACKET_SIZE_BYTES + 1;

    struct Command {
        size_t length;
        uint8_t data[MAX_COMMAND_LENGTH];

        /**
         * Constructor used to make a command object from a packet and command code.
         * Automatically computes length and checksum value.
         * `data` field will contain value to send to the Swarm device.
         * 
         * @param code (const char [COMMAND_LENGTH + 1]): 2 character command code with null-termination.
         *  for the command length.
         * @param packet (NodePacket &packet): Packet containing data used in command.
        */
       Command(const char code[COMMAND_CODE_LENGTH + 1], packets::NodePacket &packet);
    };

    // TODO: Create this
    struct Response {
    };

    enum Error {
        CMD_BADPARAM,
        CMD_BADPARAMLENGTH,
        CMD_BADPARAMVALUE,
        CMD_INVALIDCHAR,
        CMD_NOTIMPLEMENTED,
        CMD_PARAMMISSING,
        CMD_PARAMDUPLICATE,
    };

    /**
     * Struct providing the Swarm device interface.
     * @param client (WiFiClient): WiFi connection to Swarm device.
     * @param status (uint8_t): Current status of the client connection.
    */
    struct Device {
        WiFiClient client;
        uint8_t status;

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
        bool isConnected();

        /**
         * Function for connecting to the Swarm device.
         * @param server (IPAddress): IP address of WiFi client.
         * @param port (uint16_t): Port to connect to on WiFiClient.
         * @param retries (uint8_t): Maximum number of retries when trying to begin the WiFi
         *  network or connect to the Swarm device.
         * @param connectDelay (float): Number of milliseconds to wait after connecting to
         *  WiFi network and between connection attempts to Swarm access point.
        */
        bool connect(IPAddress server, uint16_t port, uint8_t retries = 3, float connectDelay = 3);

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
         * @param (packets::NodePacket): Packet being received.
        */
        void receivePacket(packets::NodePacket packet);

        /**
         * Function responsible for sending a SWARM command.
         * @param command (Command): Object containing relevant information
         *  to issue a command to the SWARM device. Should NOT include the 
         *  leading '$' or trailing checksum.
         * 
         * @returns TODO: Parse return.
        */
        void sendCommand(Command command);

        /**
         * Function which sends a string to the client until it encounters
         * a null-terminating character.
         * @param data (uint8_t []): Data to send. Does not need to be null-terminated.
         * @param length (size_t): Number of bytes to send.
        */
        void sendData(uint8_t data[], size_t length);

        /**
         * Function to read a single message from the client.
         * Reads in data until it encounters a '$' character, then
         * continues reading until it encounters a newline or end of message.
         * 
         * TODO: Finish this method. Read in one message at a time to check for:
         * a) The "$M138 DATETIME*56" message meaning we can send data.
         * b) The "$TD OK ..." message meaning our data has been accepted for sending
         * c) Any "$TD SENT ..." messages confirming data had been successfully uploaded
         *    to the satellite from the device.
         * 
         * @returns (char *): Pointer to static character array of up to MAX_MESSAGE_LENGTH.
         *  which exists within the readData function.
        */
        uint8_t* readData();

        /**
         * Function to read all data from the Swarm and print it via Serial.
        */
        void readContinuously();

    };
}

#endif /* SWARM_H */
/**
 * swarm.h
 * 
 * Header file containing functions for interacting with the modem device.
 * 
 * Created as part of the UVM CIROH project.
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
    #define MAX_MESSAGE_LENGTH 80
    #define COMMAND_PREFIX_LENGTH 3
    #define CHECKSUM_LENGTH 3

    struct Command {
        char *command;
        uint32_t length;
    };

    struct CommandResponse {

    };

    enum SwarmError {
        CMD_BADPARAM,
        CMD_BADPARAMLENGTH,
        CMD_BADPARAMVALUE,
        CMD_INVALIDCHAR,
        CMD_NOTIMPLEMENTED,
        CMD_PARAMMISSING,
        CMD_PARAMDUPLICATE,
    };

    /**
     * Function which triggers when a packet is received by a node.
     * Performs any relevant deserialization/queueing of packets
     * then sends packet(s) (if there are any to send yet) to the 
     * SWARM device/
    */
    void routePacketToSwarm(WiFiClient client, packets::NodePacket packet);

    /**
     * Function responsible for sending a SWARM command.
     * 
     * @param client (WiFiClient): SWARM WiFi client device connection.
     * @param command (Command): Object containing relevant information
     *  to issue a command to the SWARM device. Should NOT include the 
     *  leading '$' or trailing checksum.
     * 
     * @returns TODO: Parse return.
    */
    void sendCommand(WiFiClient client, Command command);

    /**
     * Function which sends a string to the client until it encounters
     * a null-terminating character.
     * 
     * @param client (WiFiClient): SWARM WiFi client device connection.
     * @param data (char *): Data to send. Does not need to be null-terminated.
     * @param length (uint32_t): Number of bytes to send.
    */
    void sendString(WiFiClient client, char *data, uint32_t length);

    /**
     * Function to read all data from the client (SWARM access point)
     * and print it via serial.
     * 
     * @param client (WiFiClient): Client device.
    */
    void readContinuously(WiFiClient client);

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
     * @param client (WiFiClient): Client device.
     * 
     * @returns (char *): Pointer to static character array of up to `MAX_MESSAGE_LENGTH`.
     *  which exists within the `readData` function.
    */
    char *readData(WiFiClient client);
}

#endif /* SWARM_H */
/**
 * packets.h
 * 
 * Header file containing declarations for dealing with data packets.
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/23/24
*/

#ifndef PACKETS_H
#define PACKETS_H

#include <ArduinoJson.h>
#include <functional>
#include <iostream>
#include <WiFi.h>

namespace packets {

    // External variables
    #define MAX_NODE_PACKET_SIZE_BYTES 192
    /**
     * Type for a packet which would be received by the device
     * containing sensor readings from nodes in the field.
     * 
     * @param length (uint8_t): Length of the actual relevant
     *  data section contained in the data array. Remainder
     *  should be skipped.
     * @param data (char *): Array of bytes containing serialized
     *  JSON data. Maximum length is the maximum allowable packet
     *  size on the Swarm.
     * @param crc (uint16_t): Checksum to verify packet was received correctly.
    */
    struct NodePacket {
        uint8_t length;
        char data[MAX_NODE_PACKET_SIZE_BYTES + 1];
        uint16_t crc;

        /**
         * Constructor sets length to 0, and last byte of data array to null byte.
        */
        NodePacket();

        /**
         * Constructor which creates a packet from arguments.
         * @param str (const char *): Byte array for the packet data.
         * @param len (size_t): Number of bytes in the array.
        */
        NodePacket(const char *str, size_t len);

        /**
         * Convenience constructor.
         * @param str (const char *): C-string containing data.
        */
        NodePacket(const char *str);

        /**
         * Print node packet fields.
        */
        void print();
    };

    // Type signature for a packet handler function
    using PacketHandler = std::function<void(NodePacket packet)>;

    // External handler functions
    extern PacketHandler RECEIVE_NODE_PACKET;

    // ---------- Functions ----------

    /**
     * Function which takes the JSON map representation of data to be transmitted
     * and turns it into a NodePacket if it can.
     * @param doc (JsonDocument): JSON document object containing just the data
     *  being transmitted. Additional metadata gets added here.
     * @returns (NodePacket) Node packet object.
    */
    NodePacket makeNodePacket(JsonDocument &doc);

    /**
     * Function which tries to deserialize a node packet's JSON object.
     * @param packet (&NodePacket): Reference to a packet.
     * @param doc (&JsonDocument): Reference to the JSON document which gets filled.
     * @returns (std::optional<JsonDocument>) JSON object if the node's data field
     *  contained correctly formatted JSON serial data, and nothing otherwise.
    */
    DeserializationError getPacketData(NodePacket &packet, JsonDocument &doc);

    /**
     * Function which gets called whenever a packet is received and calls the registered
     * handler function if there is one.
     * @param packet (NodePacket): Raw packet received from a node.
    */
    void receivePacket(NodePacket packet);

    /**
     * Function used to register a receive packet handler function.
     * @param function (PacketHandler): Function which takes in the WiFi client
     *  and the received packet then takes the appropriate action.
    */
    void registerReceivePacketHandler(PacketHandler handler);

    /**
     * Packet handler function which just prints the packet.
     * @param packet (NodePacket): Node packet being printed.
    */
    void printPacketHandler(NodePacket packet);
}

#endif /* PACKETS_H */

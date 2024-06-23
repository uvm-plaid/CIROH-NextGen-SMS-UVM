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

#include <WiFi.h>

namespace packets {
    #define MAX_NODE_PACKET_SIZE_BYTES 256

    struct NodePacket {
        char *data;
        uint32_t length;
    };

    typedef void (*PacketHandler)(WiFiClient client, NodePacket packet);

    extern PacketHandler RECEIVE_NODE_PACKET;

    void callReceiveNodePacketHandler(WiFiClient client, NodePacket packet);
    void registerReceiveNodePacketHandler(PacketHandler handler);
}

#endif /* PACKETS_H */

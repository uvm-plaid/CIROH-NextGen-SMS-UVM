/**
 * packets.cpp
 * 
 * File containing definitions used for dealing with data packets.
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/23/24
*/

#include "packets.h"

namespace packets {
    PacketHandler RECEIVE_NODE_PACKET = nullptr;

    void callReceiveNodePacketHandler(WiFiClient client, NodePacket packet) {
        if (RECEIVE_NODE_PACKET != nullptr) {
            RECEIVE_NODE_PACKET(client, packet);
        } else {
            Serial.println("No registered handler.");
        }
    }

    void registerReceiveNodePacketHandler(PacketHandler handler) {
        if (handler != nullptr) {
            RECEIVE_NODE_PACKET = handler;
        }
    }
}

/**
 * packets.cpp
 * 
 * File containing definitions used for dealing with data packets.
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/23/24
*/

#include "checksum.h"
#include "packets.h"
#include "printing.h"

#include <ArduinoJson.h>
#include <ctime>

namespace packets {
    // Handler functions are null pointers by default
    PacketHandler RECEIVE_NODE_PACKET = nullptr;

    NodePacket::NodePacket(uint8_t data[], size_t length) {
        length = length;
        data = data;
    }

    NodePacket::NodePacket() {
        length = 0;
        data[MAX_NODE_PACKET_SIZE_BYTES] = 0;
    }

    void NodePacket::print() {
        printing::dbgln("Packet Length: %d Bytes", length);
        printing::dbgln("Data: %s", (char *) data);
        printing::dbgln("CRC: 0x%02x", crc);
    }

    NodePacket makeNodePacket(JsonDocument &doc) {
        NodePacket packet{};

        // Add metadata to JSON document
        doc["time"] = std::time(nullptr);
        
        packet.length = serializeJson(doc, packet.data);
        packet.crc = checksum::packetChecksum(packet);

        return packet;
    }


    DeserializationError getPacketData(NodePacket &packet, JsonDocument &doc) {
        doc.clear();
        return deserializeJson(doc, packet.data, packet.length);
    }

    // JsonDocument getPacketData(NodePacket packet) {
    //     JsonDocument doc;
    //     DeserializationError error;

    //     // if ((error = deserializeJson(doc, packet.data, packet.length))) {
    //     //     printing::dbgln(error.c_str());
    //     // }

    //     doc["name"] = "Inner function";

    //     return doc;
    // }

    void receivePacket(NodePacket packet) {
        if (RECEIVE_NODE_PACKET != nullptr) {
            RECEIVE_NODE_PACKET(packet);
        } else {
            Serial.println("No registered handler.");
        }
    }

    void registerReceivePacketHandler(PacketHandler handler) {
        if (handler != nullptr) {
            RECEIVE_NODE_PACKET = handler;
        }
    }

    void printPacketHandler(NodePacket packet) {
        packet.print();
    }
}

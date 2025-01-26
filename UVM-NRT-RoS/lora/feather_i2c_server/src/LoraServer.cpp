#include <Arduino.h>
#include <cstring>
#include "loraDevice.h"

bool LoraServer::receive(LoraPacket &src) 
{
    size_t pointer = writer;
    noInterrupts(); 
    do {
        pointer = (pointer + 1) % MAX_NUM_PACKETS;
        if (packet_buffer[pointer].flag == LoraPacketStatus::FREE) {
            // Write the packet data into the packet buffer
            packet_buffer[pointer].packet = src;
            packet_buffer[pointer].flag = LoraPacketStatus::FULL;
            writer = pointer; 
            interrupts(); 
            return true;
        }
    } while (pointer != writer);

    interrupts(); 
    return false; 
}

bool LoraServer::packetReady() 
{
    // Just in case this got called multiple times and was on a full packet
    if (packet_buffer[reader].flag == LoraPacketStatus::FULL) {
        return true;
    }

    size_t pointer = reader;
    do {
        pointer = (pointer + 1) % MAX_NUM_PACKETS;

        if (packet_buffer[pointer].flag == LoraPacketStatus::FULL) {
            // Update the read pointer
            reader = pointer;
            return true;
        }
    } while (pointer != reader);

    // Full rotation around the array, nothing ready yet
    return false;
}

bool LoraServer::request(LoraPacket &dst) 
{
    // Check if there's a packet ready and update read pointer if necessary
    if (packetReady()) {
        noInterrupts(); // Disable interrupts to enter critical section
        
        // Check if the packet is marked as FULL
        if (packet_buffer[reader].flag == LoraPacketStatus::FULL) {
            // Mark the packet as READING
            packet_buffer[reader].flag = LoraPacketStatus::READING;

            // Read to destination buffer, then zero out memory
            dst = packet_buffer[reader].packet;
            packet_buffer[reader].packet = {};
            packet_buffer[reader].flag = LoraPacketStatus::FREE;
            interrupts();
            return true;
        }
        
        interrupts();
    }
    return false;
}


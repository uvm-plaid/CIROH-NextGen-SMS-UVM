#include <Arduino.h>
#include "LoraDevice.h"

LoraPeripheralStatus LoraPeripheral::receive(LoraPacket &src) 
{
    if (full()) {
        return LoraPeripheralStatus::BUFFER_FULL;
    }

    size_t pointer = writer;
    noInterrupts(); 
    do {
        Serial.print("Pointer: ");
        Serial.print(pointer);
        Serial.print(" Num Packets: ");
        Serial.println(num_packets);
        pointer = (pointer + 1) % MAX_NUM_PACKETS;
        if (packet_buffer[pointer].flag == LoraPacketStatus::FREE) {
            // Write the packet data into the packet buffer
            packet_buffer[pointer].packet = src;
            packet_buffer[pointer].flag = LoraPacketStatus::FULL;
            writer = pointer; 
            ++num_packets;
            interrupts(); 
            return LoraPeripheralStatus::RECEIVE_SUCCESSFUL;
        }
    } while (pointer != writer);
    interrupts(); 
    return LoraPeripheralStatus::RECEIVE_FAILED; 
}


LoraPeripheralStatus LoraPeripheral::request(LoraPacket &dst) 
{
    if (!find_packet()) {
        return LoraPeripheralStatus::BUFFER_EMPTY;
    }
    Serial.print("Pointer: ");
    Serial.print(reader);
    Serial.print(" Num Packets: ");
    Serial.println(num_packets);

    // Entering critical section
    noInterrupts();
    
    // Check if the packet is marked as FULL
    if (packet_buffer[reader].flag == LoraPacketStatus::FULL) {
        // Mark the packet as READING
        packet_buffer[reader].flag = LoraPacketStatus::READING;

        // Read to destination buffer, then zero out memory
        dst = packet_buffer[reader].packet;
        packet_buffer[reader].packet = {};
        packet_buffer[reader].flag = LoraPacketStatus::FREE;
        --num_packets;
        interrupts();
        return LoraPeripheralStatus::REQUEST_SUCCESSFUL;
    }
    
    interrupts();
    return LoraPeripheralStatus::REQUEST_FAILED;
}

bool LoraPeripheral::find_packet() {
    if (empty()) {
        return false;
    }
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

#pragma once

#include <stdint.h>
#include <stddef.h>

class LoraDevice
{
public:
    virtual void setup() = 0;
    virtual int32_t recv(uint8_t* buffer, uint8_t length) = 0;
    virtual int32_t send(const uint8_t* buffer, uint8_t length) = 0;
    virtual bool isAvailable() = 0;
    virtual int16_t lastRssi() = 0;
    virtual float lastSnr() = 0;

    virtual void setCrcEnabled(bool enabled) = 0;

    static LoraDevice* getInstance() { return Instance; }

private:
    static LoraDevice* Instance;
};

enum class LoraPacketStatus {
    FREE,
    WRITING,
    READING,
    FULL,
};

struct LoraPacket {
    uint8_t data[32];

    LoraPacket() : data{0} {}
};

struct LoraPacketEntry {
    volatile LoraPacketStatus flag; 
    LoraPacket packet;

    LoraPacketEntry() 
        : flag{LoraPacketStatus::FREE}, 
          packet{} {}
};


const static size_t MAX_NUM_PACKETS = 10;

class LoraServer {
public:
    LoraServer() : packet_buffer{}, writer{0}, reader{0} {}

    /**
     * Receive a lora packet into the server. First mark intent to do so by
     * setting flag in destination.
     *
     * @param src (&LoraPacket): Source buffer packet.
     *
     * @returns (bool): True if the value was received, false otherwise.
     */
    bool receive(LoraPacket &src);

    /**
     * Read a lora packet into a destination buffer, and clear out the existing
     * entry once it's done.
     *
     * @param dst (&LoraPacket): Destination buffer for a packet.
     *
     * @returns (bool): True if there was a value to be read, false otherwise.
     */
    bool request(LoraPacket &dst);

    /**
     * Peek ahead to see if there is data.
     * Also positions the read pointer.
     *
     * @returns (bool): True if a packet is ready, false otherwise.
     */
    bool packetReady();

private:
    LoraPacketEntry packet_buffer[MAX_NUM_PACKETS];
    // Read/write pointers into the packet buffer
    size_t writer;
    size_t reader;
};


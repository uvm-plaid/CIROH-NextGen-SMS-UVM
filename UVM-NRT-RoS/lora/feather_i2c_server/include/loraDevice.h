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

/**
 * Flags for an entry in the lora server class.
 */
enum class LoraPacketStatus: uint8_t {
    FREE,
    WRITING,
    READING,
    FULL,
};

/**
 * Flags for potential return values from a lora server operation.
 */
enum class LoraServerStatus: uint8_t {
    BUFFER_FULL,
    BUFFER_EMPTY,
    RECEIVE_SUCCESSFUL,
    RECEIVE_FAILED,
    REQUEST_SUCCESSFUL,
    REQUEST_FAILED,
};

/**
 * Flags for a lora packet being transmitted.
 */
enum class LoraPacketFlags: uint8_t {
    // Flags for communicating with host microcontroller
    NO_MORE_PACKETS,
    // Flags for between lora devices
};

/**
 * A class representing a Lora Packet which will be transmitted between Lora
 * modules and to a host microcontroller. Contains sensor readings, and any
 * communication flags.
 */
class LoraPacket {
public:
    // TODO: Firm up actual binary layout with correct fields/sizes
    uint8_t source_id;
    uint16_t sonar;
    uint16_t solar_radiation;
    uint16_t temperature;
    uint16_t relative_humidity;
    uint16_t snow_depth;
    uint8_t phase_prediction;
    uint8_t intensity_prediction;
    LoraPacketFlags flags;

    LoraPacket() {}
};

constexpr size_t I2C_BUFFER_SIZE = 32;
static_assert(sizeof(LoraPacket) <= I2C_BUFFER_SIZE, "Lora packet must fit within the I2C buffer.");

/**
 * Struct representing an entry in the Lora Server packet buffer.
 * Contains a single lora packet along with flags used for bookkeeping
 * such as the current status of an entry (e.g., Is it currently being written
 * to when an interrupt fires).
 */
class LoraPacketEntry {
public:
    volatile LoraPacketStatus flag; 
    LoraPacket packet;

    LoraPacketEntry() 
        : flag{LoraPacketStatus::FREE}, 
          packet {} {}
};

const static size_t MAX_NUM_PACKETS = 10;

/**
 * Struct representing a Lora Server which will be running on the root Lora
 * device that will be requested for data by a connected microcontroller to
 * extract any packets which it has received and queued.
 */
class LoraServer {

private:
    LoraPacketEntry packet_buffer[MAX_NUM_PACKETS];
    // Bookkeeping information
    size_t num_packets;
    // Read/write pointers into the packet buffer
    size_t writer;
    size_t reader;

    /**
     * Function which positions the read pointer over a packet if there is one
     * available. If no such packet is available, return false.
     *
     * @returns (bool): Whether a packet could be found.
     */
    bool find_packet();

    // Singleton
    static LoraServer* instance;
    LoraServer() : 
        num_packets{0},
        writer{0},
        reader{0} {}

public:

    static LoraServer* get_instance() { 
        if (!instance) {
            instance = new LoraServer();
        }
        return instance; 
    }

    inline bool empty() { return num_packets == 0; }
    inline bool full() { return num_packets == MAX_NUM_PACKETS; }

    /**
     * Receive a lora packet into the server. First mark intent to do so by
     * setting flag in destination.
     *
     * @param src (&LoraPacket): Source buffer packet.
     *
     * @returns (LoraServerStatus): Status result of operation.
     */
    LoraServerStatus receive(LoraPacket &src);

    /**
     * Read a lora packet into a destination buffer, and clear out the existing
     * entry once it's done.
     *
     * @param dst (&LoraPacket): Destination buffer for a packet.
     *
     * @returns (LoraServerStatus): Status result of operation.
     */
    LoraServerStatus request(LoraPacket &dst);

};


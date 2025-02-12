#pragma once

#include <stdint.h>
#include <stddef.h>

struct LoraDevice
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
 * Flags for an entry in the lora server struct.
 */
enum struct LoraPacketStatus: uint8_t {
    FREE,
    WRITING,
    READING,
    FULL,
};

/**
 * Flags for potential return values from a lora server operation.
 */
enum struct LoraPeripheralStatus: uint8_t {
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
enum struct LoraPacketFlags: uint8_t {
    // Flags for communicating with host microcontroller
    NO_MORE_PACKETS = 1,
    // Flags for between lora devices
};

/**
 * A struct representing a Lora Packet which will be transmitted between Lora
 * modules and to a host microcontroller. Contains sensor readings, and any
 * communication flags.
 */
struct LoraPacket {
    public:
    uint8_t magic = LoraPacket::MAGIC;

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

    const static uint8_t MAGIC = 0x7F;

    // SerDe = Serialization Deserialization
    enum class SerDeStatus {
        InsufficientBufferSize,
        InvalidMagicNumber,
        Valid,
    };

    LoraPacket() = default;

    static LoraPacket deserialize(const uint8_t buf[], uint32_t buf_size, uint32_t &index, LoraPacket::SerDeStatus &status);
    LoraPacket::SerDeStatus serialize(uint8_t buf[], uint32_t buf_size, uint32_t &index);
};

/**
 * Struct representing an entry in the Lora Server packet buffer.
 * Contains a single lora packet along with flags used for bookkeeping
 * such as the current status of an entry (e.g., Is it currently being written
 * to when an interrupt fires).
 */
struct LoraPacketEntry {
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
struct LoraPeripheral {

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
    static LoraPeripheral* instance;
    LoraPeripheral() : 
        num_packets{0},
        writer{0},
        reader{0} {}

public:

    static LoraPeripheral* get_instance() { 
        if (!instance) {
            instance = new LoraPeripheral();
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
     * @returns (LoraPeripheralStatus): Status result of operation.
     */
    LoraPeripheralStatus receive(LoraPacket &src);

    /**
     * Read a lora packet into a destination buffer, and clear out the existing
     * entry once it's done.
     *
     * @param dst (&LoraPacket): Destination buffer for a packet.
     *
     * @returns (LoraPeripheralStatus): Status result of operation.
     */
    LoraPeripheralStatus request(LoraPacket &dst);

};


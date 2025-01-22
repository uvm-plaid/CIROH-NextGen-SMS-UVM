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

    enum class SerdeStatus {
        InsufficientBufferSize,
        InvalidMagicNumber,
        Valid,
    };

    LoraPacket() = default;

    static LoraPacket fromBuffer(const uint8_t buf[], uint32_t buf_size, uint32_t &index, LoraPacket::SerdeStatus &status) {
        auto get_u8 = [&buf, &index]() -> uint8_t { return buf[index++]; };
        auto get_u16 = [get_u8, &buf, &index]() -> uint16_t {
            // Assume little endian byte ordering
            uint8_t low = get_u8();
            uint8_t high = get_u8();
            uint16_t value = (high << 8) | low;
            return value;
        };
        LoraPacket packet;
        if ((buf_size - index) < sizeof(LoraPacket)) {
            status = LoraPacket::SerdeStatus::InsufficientBufferSize;
            return packet; 
        } else if (get_u8() != LoraPacket::MAGIC) {
            status = LoraPacket::SerdeStatus::InvalidMagicNumber;
            return packet; 
        }
        packet.source_id = get_u8();
        packet.sonar = get_u16();
        packet.solar_radiation = get_u16();
        packet.temperature = get_u16();
        packet.relative_humidity = get_u16();
        packet.snow_depth = get_u16();
        packet.phase_prediction = get_u8();
        packet.intensity_prediction = get_u8();
        packet.flags = static_cast<LoraPacketFlags>(get_u8());
        status = LoraPacket::SerdeStatus::Valid;
        return packet;
    }
    LoraPacket::SerdeStatus toBuffer(uint8_t buf[], uint32_t buf_size, uint32_t &index) {
        auto write_u8 = [&buf, &index](uint8_t value) {
            buf[index++] = value;
        };
        auto write_u16 = [write_u8, &buf, &index](uint16_t value) {
            // Assume little endian byte ordering
            write_u8(static_cast<uint8_t>(value & 0xFF));
            write_u8(static_cast<uint8_t>((value >> 8) & 0xFF));
        };
        if ((buf_size - index) < sizeof(LoraPacket)) {
            return LoraPacket::SerdeStatus::InsufficientBufferSize;
        } else if (this->magic != LoraPacket::MAGIC) {
            return LoraPacket::SerdeStatus::InvalidMagicNumber;
        }
        buf[index] = this->magic;
        write_u8(this->magic);
        write_u16(this->sonar);
        write_u16(this->solar_radiation);
        write_u16(this->temperature);
        write_u16(this->relative_humidity);
        write_u16(this->snow_depth);
        write_u8(this->phase_prediction);
        write_u8(this->intensity_prediction);
        write_u8(static_cast<uint8_t>(this->flags));
        
        return LoraPacket::SerdeStatus::Valid;
    }

};

constexpr size_t I2C_BUFFER_SIZE = 32;
static_assert(sizeof(LoraPacket) <= I2C_BUFFER_SIZE, "Lora packet must fit within the I2C buffer.");

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


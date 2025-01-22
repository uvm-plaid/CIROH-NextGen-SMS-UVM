#include <loraDevice.h>

LoraPacket LoraPacket::deserialize(const uint8_t buf[], uint32_t buf_size, uint32_t &index, LoraPacket::SerdeStatus &status) {
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


LoraPacket::SerdeStatus LoraPacket::serialize(uint8_t buf[], uint32_t buf_size, uint32_t &index) {
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

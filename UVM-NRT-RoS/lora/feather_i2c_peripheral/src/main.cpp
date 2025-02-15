#include "LoraDevice.h"
#include "tdm.h"
#include <Arduino.h>
#include <Wire.h>

#if defined(USING_SX1276) || defined(USING_SX1278)
#include <U8g2lib.h>
#include "LoRaBoards.h"
#else
#include "rf95.h"
#endif

constexpr uint32_t DeviceID = 0;
constexpr bool isGateway = (DeviceID == 0);
constexpr bool isPeripheral = isGateway;
constexpr uint8_t peripheral_address = 0x08;  // Hardcoded for now

static LoraDevice* lora = nullptr;
LoraPeripheral* LoraPeripheral::instance = nullptr;
static LoraPeripheral* peripheral = nullptr;

/**
 * This function is called whenever a device running the I2C controller
 * requests from the feather.
 * 1) Requests a packet from the packet buffer (early return if empty).
 * 2) Serializes packet into buffer (eliminate any padding from compiler).
 * 3) Write buffer to receiver.
 */
void i2c_request_handler() {
    static uint8_t buf[sizeof(LoraPacket) + 1];
    LoraPacket packet;
    LoraPeripheralStatus status = peripheral->request(packet);
    if (status == LoraPeripheralStatus::BUFFER_EMPTY) {
        packet.flags = LoraPacketFlags::NO_MORE_PACKETS;
        return;
    }
    uint32_t nbytes = 0;
    LoraPacket::SerDeStatus ret = packet.serialize(buf, sizeof(buf), nbytes);
    Serial.println(static_cast<uint8_t>(status));
    if (ret == LoraPacket::SerDeStatus::Valid) {
        Wire.write(buf, nbytes);
    }
}

void i2c_initiate() {
    peripheral = LoraPeripheral::get_instance();
    if (isPeripheral) {
        Wire.begin(peripheral_address);
        Wire.onRequest(i2c_request_handler);
        Serial.println("Setup I2C server.");
    } else {
        // This code should only be running on a server so loop
        while (true);
    }
}


void setup() {
    i2c_initiate();
}

void loop() {
    LoraPacket packet;
    for (int i = 0; i <= 10; ++i) {
        packet.source_id = i; 
        LoraPeripheralStatus status = peripheral->receive(packet);
        if (status == LoraPeripheralStatus::RECEIVE_SUCCESSFUL) {
            Serial.println("Successfully received packet");
        }
        delay(i * 50);
    }
}


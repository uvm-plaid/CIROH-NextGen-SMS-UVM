// Only supports SX1276/SX1278
#include <LoRa.h>
#include "LoRaBoards.h"
#include "LilyGoLoraDevice.h"

#ifndef CONFIG_RADIO_FREQ
#define CONFIG_RADIO_FREQ           915.0
#endif
#ifndef CONFIG_RADIO_OUTPUT_POWER
#define CONFIG_RADIO_OUTPUT_POWER   17
#endif
#ifndef CONFIG_RADIO_BW
#define CONFIG_RADIO_BW             125.0
#endif

#if !defined(USING_SX1276) && !defined(USING_SX1278)
#error "LoRa example is only allowed to run SX1276/78. For other RF models, please run examples/RadioLibExamples"
#endif

// from lilygo examples
void LilyGoLoraDevice::setup()
{
    setupBoards();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("LoRa Receiver");

#ifdef  RADIO_TCXO_ENABLE
    pinMode(RADIO_TCXO_ENABLE, OUTPUT);
    digitalWrite(RADIO_TCXO_ENABLE, HIGH);
#endif

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(CONFIG_RADIO_FREQ * 1000000)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    LoRa.setTxPower(CONFIG_RADIO_OUTPUT_POWER);

    LoRa.setSignalBandwidth(CONFIG_RADIO_BW * 1000);

    LoRa.setSpreadingFactor(10);

    LoRa.setPreambleLength(16);

    LoRa.setSyncWord(0xAB);

    LoRa.enableCrc();

    LoRa.disableInvertIQ();

    LoRa.setCodingRate4(7);
}

int32_t LilyGoLoraDevice::recv(uint8_t* buffer, uint8_t length)
{
    int packetLength = LoRa.parsePacket();

    // No idea if readBytes() exits early if fewer than `length` bytes
    // are in the received packet, so forcing the issue for now.
    int32_t bytesRead = (int32_t)LoRa.readBytes(buffer, length > packetLength ? packetLength : length);
    if (bytesRead == 0) {
        return 0;
    }

    if (bytesRead < 4) {
        // Invalid packet received.
        return -2;
    }
    // There's a 4 byte header that we're ignoring for now.
    memmove(buffer, buffer + 4, bytesRead - 4);
    return bytesRead - 4;
}

int32_t LilyGoLoraDevice::send(const uint8_t* buffer, uint8_t length)
{
    if (!LoRa.beginPacket()) {
        return -1;
    }
    // Write 4 extra bytes for the header
    uint32_t header = 0;
    LoRa.write((uint8_t*)&header, sizeof(header));
    int written = LoRa.write(buffer, length);
    LoRa.endPacket();
    // Return bytes written, not including 4 byte header.
    return written;
}

bool LilyGoLoraDevice::isAvailable()
{
    return LoRa.available();
}

int16_t LilyGoLoraDevice::lastRssi()
{
    return LoRa.packetRssi();
}

float LilyGoLoraDevice::lastSnr()
{
    return LoRa.packetSnr();
}

void LilyGoLoraDevice::setCrcEnabled(bool enabled)
{
    if (enabled) LoRa.enableCrc();
    else LoRa.disableCrc();
}

/*
void loop()
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        String recv = "";
        // read packet
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }

        Serial.println(recv);

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
        if (u8g2) {
            u8g2->clearBuffer();
            char buf[256];
            u8g2->drawStr(0, 12, "Received OK!");
            u8g2->drawStr(0, 26, recv.c_str());
            snprintf(buf, sizeof(buf), "RSSI:%i", LoRa.packetRssi());
            u8g2->drawStr(0, 40, buf);
            snprintf(buf, sizeof(buf), "SNR:%.1f", LoRa.packetSnr());
            u8g2->drawStr(0, 56, buf);
            u8g2->sendBuffer();
        }
    }
}
*/

// Set static variables
static LilyGoLoraDevice device;
LoraDevice* LoraDevice::Instance = &device;

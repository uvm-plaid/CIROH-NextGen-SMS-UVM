#include "FeatherLoraDevice.h"
#include "rf95.h"
#include <Arduino.h>

void FeatherLoraDevice::setup()
{
    while (!Serial);
    Serial.println("Serial is ready.");
    basic_setup(0, 0);
    Serial.println("Setup complete!");
}

int32_t FeatherLoraDevice::recv(uint8_t* buffer, uint8_t length) {
    if (rf95.recv(buffer, &length)) {
        return length;
    } else {
        return -1;
    }
}

int32_t FeatherLoraDevice::send(const uint8_t* buffer, uint8_t length) {
    bool success = rf95.send(buffer, length);
    rf95.waitPacketSent();
    return success;

    if (rf95.send(buffer, length)) {
        rf95.waitPacketSent();
        return length;
    } else {
        return -1;
    }
}

bool FeatherLoraDevice::isAvailable() {
    return true;
}

int16_t FeatherLoraDevice::lastRssi() {
    return rf95.lastRssi();
}

float FeatherLoraDevice::lastSnr() {
    return rf95.lastSNR();
}

void FeatherLoraDevice::setCrcEnabled(bool enabled) {
    rf95.setPayloadCRC(enabled);
}

void basic_setup(uint8_t thisAddress, uint8_t toAddress) {
    #ifdef ARDUINO_SAMD_MKRWAN1310
    #else
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(100);

  Serial.println("Feather LoRa RX Test!");

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);

  // Other devices don't have a way of handling addresses,
  // so set promiscuous to keep behavior consistant across devices.
  rf95.setCodingRate4(7);
  rf95.setSpreadingFactor(10);
  rf95.setPreambleLength(16);
  // This is how you set the sync word!!!
  rf95.spiWrite(RH_RF95_REG_39_SYNC_WORD, 0xAB);
  rf95.setPromiscuous(true);
  //rf95.setSignalBandwidth(RH_RF95_BW_125KHZ);
  //rf95.setTxPower(17);
  //rf95.spiWrite(RH_RF95_REG_33_INVERT_IQ, RH_RF95_IQ);
  //rf95.setThisAddress(thisAddress);
  //rf95.setHeaderFrom(thisAddress);
  //rf95.setHeaderTo(toAddress);
  #endif
}

FeatherLoraDevice device;
LoraDevice* LoraDevice::Instance = &device;

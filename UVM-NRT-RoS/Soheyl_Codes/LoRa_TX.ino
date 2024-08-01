#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define LED 13 // Built-in LED
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(100);

  Serial.println("Feather LoRa TX Test!");

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
}

int16_t packetnum = 0;

void loop() {
  delay(1000);
  Serial.println("Transmitting...");

  char radiopacket[20] = "Hello Girls#      ";
  itoa(packetnum++, radiopacket + 13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  radiopacket[19] = 0;

  rf95.send((uint8_t *)radiopacket, 20);
  rf95.waitPacketSent();

// Blink LED to indicate data sent
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  
  Serial.println("Packet sent!");
}

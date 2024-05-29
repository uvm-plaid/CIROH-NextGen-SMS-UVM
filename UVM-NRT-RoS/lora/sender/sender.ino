#include <SPI.h>
#include <LoRa.h>

int counter = 0;
int button = 2;
int buttonState;

void setup() {
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);

  while (!Serial);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  delay(1000);
}

void loop() {
  buttonState = digitalRead(button);

  if (buttonState == LOW) {
    LoRa.beginPacket();
    LoRa.print("button pressed");
    LoRa.endPacket();

    counter++;
    
    Serial.print("Sending packet: ");
    Serial.println(counter);
    delay(500);
  }
}

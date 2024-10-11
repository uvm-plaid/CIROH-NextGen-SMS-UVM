#include "HardwareSerial.h"
#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Begin write");
}

void loop() {
  Serial.println("loop");
  Wire.beginTransmission(1);
  Wire.write("Hello World!");
  Wire.endTransmission();

  delay(2000);
}

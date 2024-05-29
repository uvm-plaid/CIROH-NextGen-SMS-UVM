#include "HardwareSerial.h"
#include <Arduino.h>
#include <Wire.h>

void receiveEvent(int howMany) {
  int numBytes = Wire.available();
  if (numBytes == 0) {
    Serial.println("No data");
    return;
  }

  char* message = (char*)malloc(numBytes+1 * sizeof(char));
  if (message == NULL) {
    Serial.println("Failed to allocate memory");
    return;
  }

  for (int i = 0;  i < numBytes; i++) {
    char c = Wire.read();
    message[i] = c;
  }
 
  message[numBytes] = '\0';
  Serial.println(message);

  free(message);
}

void setup() {
  Serial.begin(9600);
  Wire.begin(2);
  Wire.onReceive(receiveEvent);
}

void loop() {}

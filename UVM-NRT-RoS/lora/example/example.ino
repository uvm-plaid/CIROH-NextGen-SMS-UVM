#include <Arduino.h>

int delayTime = 300;   // milliseconds, so 1000 = one second
int LED_BUILTIN_green = 8;  // Mayfly has a green LED connected to digital pin 8
int LED_BUILTIN_red = 9;    // Mayfly has a red LED connected to digital pin 9

void setup() {
  Serial.begin(57600);
  pinMode(LED_BUILTIN_green, OUTPUT);  // set the pin as output to turn on the LED
  pinMode(LED_BUILTIN_red, OUTPUT);    // all you need to set up the LED blink
  delay(delayTime);
  Serial.println("Mayfly: Blink demo 1");  // sends a message to the Serial Monitor
  delay(delayTime); 
}

void loop() {
  Serial.println("looping");
  digitalWrite(LED_BUILTIN_green, HIGH);
  digitalWrite(LED_BUILTIN_red, LOW);     
  delay(delayTime);   
  digitalWrite(LED_BUILTIN_green, LOW);
  delay(delayTime);
  digitalWrite(LED_BUILTIN_green, HIGH);
  delay(delayTime);
  digitalWrite(LED_BUILTIN_red, HIGH);
  digitalWrite(LED_BUILTIN_green, LOW);
  delay(delayTime);
  digitalWrite(LED_BUILTIN_red, LOW);
  delay(delayTime);
  digitalWrite(LED_BUILTIN_red, HIGH);
  delay(delayTime);
}

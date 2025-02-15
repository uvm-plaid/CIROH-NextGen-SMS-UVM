#include <Arduino.h>

#if defined(USING_SX1276) || defined(USING_SX1278)
#include <U8g2lib.h>
#include "LoRaBoards.h"
#else
#include "rf95.h"
#endif

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Setup complete!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hi there!");

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

#if 0
#include <Arduino.h>

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
#endif
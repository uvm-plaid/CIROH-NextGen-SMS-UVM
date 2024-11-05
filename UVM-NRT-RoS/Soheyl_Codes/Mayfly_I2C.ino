#include <SPI.h>
#include <Wire.h>

#define SLAVE_ADDRESS 5

float data = 25; // The data to be sent


void setup()
{    
  Wire.begin();
  Serial.begin(9600);

}

void loop()
{ 
  if(handshake())
  {
    transferlora(data);
  
  }
 delay (1000);
}
////////////////////////////////////////////////////////////

bool handshake() {
  float handshakeValue = 1; // Number for handshake
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write((byte *)&handshakeValue, sizeof(handshakeValue));
  Wire.endTransmission();

  delay(100);

  Wire.requestFrom(SLAVE_ADDRESS, 1);
  if (Wire.available()) {
    int response = Wire.read();
    if (response == 10) {
      //Handshake successful
      return true;
    } else {
      //Handshake failed
      return false;
    }
  }
  return false;
}
////////////////////////////////////////////////////////////////
void transferlora(float measure) {
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write((byte *)&measure, sizeof(measure));
    Wire.endTransmission();
    Serial.print("Data sent:");
    Serial.println(data);
    delay(35);
}


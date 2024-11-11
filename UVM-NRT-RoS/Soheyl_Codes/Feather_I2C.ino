#include <stdio.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#define SLAVE_ADDRESS 5

float receivedData;
int Sent = 10;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(DataRequest);

}




void receiveData(int numBytes) {
  if (numBytes == sizeof(receivedData)) {
    Wire.readBytes((byte*)&receivedData, sizeof(receivedData));
    
    if(receivedData != 1)
    {
      Serial.println(receivedData);
    }
    if(receivedData == 1)
    {
      Serial.println("HandShake");
    }

  }
  
}
void DataRequest()
{
  Wire.write(Sent);
}
void loop() {

}

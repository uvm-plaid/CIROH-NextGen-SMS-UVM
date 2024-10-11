#include <Arduino.h>
#include <Wire.h>
#include "HardwareSerial.h"
#include "LoraMesher.h"
#include "entities/routingTable/RouteNode.h"

LoraMesher& radio = LoraMesher::getInstance();

void printRoutingTable() {
    LM_LinkedList<RouteNode>* routingTableList = radio.routingTableListCopy();
    routingTableList->setInUse();

    for (int i = 0; i < radio.routingTableSize(); i++) {
        RouteNode* rNode = (*routingTableList)[i];
        NetworkNode node = rNode->networkNode;
        Serial.printf("|%X(%d)->%X\n", node.address, node.metric, rNode->via);
    }

    routingTableList->releaseInUse();
    delete routingTableList;
}

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
  
  RouteNode* gateway = radio.getClosestGateway();
  if (gateway == NULL) {
    printRoutingTable();
    Serial.println("Could not find gateway");
    return;
  }

  radio.createPacketAndSend(gateway->networkNode.address, message, numBytes+1);
  Serial.println("send!");

  free(message);
}
 

void setupLoraMesher() {
    radio.begin();
    radio.start();
    Serial.println("Lora Initialized");
}

void setup() {
  Serial.begin(9600);
  setupLoraMesher();
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
}


void loop() {
  if (radio.routingTableSize() == 0) {
    Serial.println("no routes");
    vTaskDelay(6000 / portTICK_PERIOD_MS);
  } else {
    printRoutingTable();
    vTaskDelay(6000 / portTICK_PERIOD_MS);
  }
}


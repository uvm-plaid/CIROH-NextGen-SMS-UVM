#include <Arduino.h>
#include "LoraMesher.h"

LoraMesher& radio = LoraMesher::getInstance();

void setupLoraMesher() {
    radio.begin();
    radio.start();
    Serial.println("Lora initialized");
}

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  setupLoraMesher();
}

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

void loop() {
  if (radio.routingTableSize() == 0) {
    Serial.println("no routes");
    vTaskDelay(6000 / portTICK_PERIOD_MS);
  } else {
    printRoutingTable();
    vTaskDelay(6000 / portTICK_PERIOD_MS);
  }
}

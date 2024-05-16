#include <Arduino.h>
#include <Wire.h>
#include "LoraMesher.h"

LoraMesher& radio = LoraMesher::getInstance();

char* extractPayload(AppPacket<char>* packet) {
    Serial.printf("Packet arrived from %X with size %d\n", packet->src, packet->payloadSize);

    char* message = packet->payload;
    Serial.println(message);
    return message;
}

void processReceivedPackets(void*) {
    for (;;) {
        ulTaskNotifyTake(pdPASS, portMAX_DELAY);

        while (radio.getReceivedQueueSize() > 0) {
            AppPacket<char>* packet = radio.getNextAppPacket<char>();
            char* message = extractPayload(packet);
            Wire.beginTransmission(2);
            Serial.println("sending over wire");
            Wire.write(message);
            Wire.endTransmission();
            radio.deletePacket(packet);
        }
    }
}

TaskHandle_t receiveLoRaMessage_Handle = NULL;

void createReceiveMessages() {
    int res = xTaskCreate(
        processReceivedPackets,
        "Receive App Task",
        4096,
        (void*) 1,
        2,
        &receiveLoRaMessage_Handle);
    if (res != pdPASS) {
        Serial.printf("Error: Receive App Task creation gave error: %d\n", res);
    }

    radio.setReceiveAppDataTaskHandle(receiveLoRaMessage_Handle);
}

void setupLoraMesher() {
    radio.begin();
    radio.start();
    radio.addGatewayRole();
    Serial.println("Lora initialized");
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  setupLoraMesher();
  createReceiveMessages();
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

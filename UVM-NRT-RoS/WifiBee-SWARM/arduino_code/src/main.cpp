/**
 * main.cpp
 * 
 * UVM CIROH Research
 * 
 * Main file for running the program operating on the Arduino Giga device
 * which connects to the Swarm via Wifi.
 * 
 * Author: Lars Jensen, Jordan Bourdeau
*/

#include <SPI.h>
#include <WiFi.h>

#include "credentials.h" 
#include "printing.h"
#include "modem.h"


// To connect to the Access Point on the SWARM (how you actually upload the data)
WiFiClient client;
IPAddress server(192, 168, 4, 1);
int port = 23;
int status = WL_IDLE_STATUS;

void setup() {

  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SSID, PASSWORD);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // You're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  // Now we'll try and open a connection to the Access Point on the wifi to the swarm
  Serial.println("\nStarting Access Point Connection...");
  if (client.connect(server, port)) {
    Serial.println("connected");
  }

  // TODO: need to read data until we get "$M138 DATETIME*56" message, once we do we can transmit data
  // TODO: send "$TD ..." message with our data
  // TODO: wait for "$TD Okay message" received to confirm our message was sent
}

void loop() {

  int index = 0;
  char helloWorld[22] = "$TD \"Hello World!\"*31";

  // Read in commands from serial to either read data coming in or send out message
  if(Serial.available()){                
    switch(Serial.read()){
      case 's':
        Serial.println("Sending Hello World");
        client.flush();
        while (helloWorld[index]) {
          client.print(helloWorld[index]);
          index++;
        }
        Serial.println("Hello World Message Sent");
        index = 0;
        break; 
      case 'f':
        flushRssi(client);
        break;
      case 'r':
        readContinuously(client);
        break;   
    }
  }

  // If we've lost the connection, disconnect from the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // Do nothing forevermore:
    while (true);
  }

}

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
#include "swarm.h"

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
    Serial.println(SWARM_SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SWARM_SSID, SWARM_PASSWORD);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // You're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  // Now we'll try and open a connection to the Access Point on the wifi to the swarm
  Serial.println("\nStarting Access Point Connection...");
  while (!client.connect(server, port)) {
    Serial.println("Trying to connect");
    delay(3000);
  }
  Serial.println("Connected!");

  // TODO: need to read data until we get "$M138 DATETIME*56" message, once we do we can transmit data
  // TODO: send "$TD ..." message with our data
  // TODO: wait for "$TD Okay message" received to confirm our message was sent
}

void loop() {
  Command command = {
    "TD \"Hello World!\"", // Command
    17, // Length
  };

  // Read in commands from serial to either read data coming in or send out message
  if(Serial.available()){                
    switch(Serial.read()){
      case 's':
        sendCommand(client, command);
        Serial.println("Hello World Message Sent");
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

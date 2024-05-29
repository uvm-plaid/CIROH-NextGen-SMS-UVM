// Created by Lars Jensen 2024
// For UVM Research 

#include <SPI.h>
#include <WiFi.h>
#include "arduino_secrets.h" 

// ------------------------------------------------------------------------------
// Please enter your sensitive data in the Secret tab/arduino_secrets.h
// ------------------------------------------------------------------------------

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status

// ------------------------------------------------------------------------------
// To connect to the Access Point on the SWARM (how you actually upload the data)
// ------------------------------------------------------------------------------

WiFiClient client;
IPAddress server(192,168,4,1);
int port = 23;


void setup() {

  // ------------------------------------------------------------------------------
  // Initialize serial and wait for port to open:
  // ------------------------------------------------------------------------------

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // ------------------------------------------------------------------------------
  // check for the WiFi module:
  // ------------------------------------------------------------------------------

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // ------------------------------------------------------------------------------
  // attempt to connect to WiFi network:
  // ------------------------------------------------------------------------------

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);

    // ------------------------------------------------------------------------------
    // Connect to WPA/WPA2 network:
    // ------------------------------------------------------------------------------
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // ------------------------------------------------------------------------------
  // you're connected now, so print out the data:
  // ------------------------------------------------------------------------------

  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  // --------------------------------------------------------------------------------
  // Now we'll try and open a connection to the Access Point on the wifi to the swarm
  // ------------------------------------------------------------------------------

  Serial.println("\nStarting Access Point Connection...");
  if (client.connect(server, port)) {
    Serial.println("connected");
  }

  // --------------------------------------------------------------------------------
  // TODO: need to read data until we get "$M138 DATETIME*56" message, once we do we can transmit data
  // --------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------
  // TODO: send "$TD ..." message with our data
  // --------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------
  // TODO: wait for "$TD Okay message" received to confirm our message was sent
  // --------------------------------------------------------------------------------


}

void loop() {

  int index = 0;
  char helloWorld[22] = "$TD \"Hello World!\"*31";

  // --------------------------------------------------------------------------------
  // Read in commands from serial to either read data coming in or send out message
  // --------------------------------------------------------------------------------

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
      case 'r':
        readContinously();
        break;   
    }
  }

  // --------------------------------------------------------------------------------
  // If we've lost the connection, disconnect from the client
  // --------------------------------------------------------------------------------

  if (!client.connected()) {

    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // --------------------------------------------------------------------------------
    // do nothing forevermore:
    // --------------------------------------------------------------------------------

    while (true);
  }

}

// --------------------------------------------------------------------------------
// TODO: Finish this method. Ideally my hope was that this would allow us to read
// in one message at a time, so we can check for:
//  a) the "$M138 DATETIME*56" message that means we can send data
//  b) the "$TD OK ..." message which means that our data had been accepted for sending
//  c) potentially any "$TD SENT ..." messages that confirm the data had been
//     successfuly uploaded to the satellite from the SWARM
// --------------------------------------------------------------------------------

char* readData () {
  static char message[80]; // Static ensures the array persists beyond the function call
  int index = 0; // Initialize index to 0
  
  if (client.available()) {
    char c = client.read();
    
    // read in everything until we get '$'
    while (c != '$') {
      c = client.read();
    }
    
    // Now that we have encountered '$', start reading the message
    while (c != '$' && index < 79) { // Read until newline or end of message, limit to 79 characters to leave space for null terminator
      message[index++] = c; // Add character to message and increment index
      c = client.read(); // Read the next character
    }
    
    // Add null terminator to mark the end of the string
    message[index] = '\0';
  }
  
  Serial.println(message);
  return message;
}

// --------------------------------------------------------------------------------
// Read in all data from the client (SWARM access point)
// --------------------------------------------------------------------------------

void readContinously(){
  while (client.available()) {

    char c = client.read();
    if (c == '$') {
      Serial.println();
    }
    Serial.print(c);
  }
}

// --------------------------------------------------------------------------------
// Method to Calculate checksum to attach at end of message
// --------------------------------------------------------------------------------

uint8_t nmeaChecksum (const char *sz, size_t len)
{
  size_t i = 0;
  uint8_t cs;

  if (sz [0] == '$'){
    i++;
  }

  for (cs = 0; (i < len) && sz [i]; i++){
    cs ^= ((uint8_t) sz [i]);
  }
  
  return cs;
}


// --------------------------------------------------------------------------------
// WiFi Methods taken from Arduino Examples
// --------------------------------------------------------------------------------

 /*
 Circuit:
 * GIGA R1 WiFi

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 modified 22 March 2023
 by Karl Söderby
 */

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}




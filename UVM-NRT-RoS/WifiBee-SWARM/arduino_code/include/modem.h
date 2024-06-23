/**
 * modem.h
 * 
 * Header file containing functions for interacting with the modem device.
 * 
 * Created as part of the UVM CIROH project.
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/16/24 
*/

#ifndef MODEM_H
#define MODEM_H

#include <stdint.h>
#include <WiFi.h>

#define MAX_MESSAGE_LENGTH 80

void flushRssi(WiFiClient client);

/**
 * Function to read all data from the client (SWARM access point)
 * and print it via serial.
 * 
 * @param client (WiFiClient): Client device.
*/
void readContinuously(WiFiClient client);

/**
 * Function to read a single message from the client.
 * Reads in data until it encounters a '$' character, then
 * continues reading until it encounters a newline or end of message.
 * 
 * TODO: Finish this method. Read in one message at a time to check for:
 * a) The "$M138 DATETIME*56" message meaning we can send data.
 * b) The "$TD OK ..." message meaning our data has been accepted for sending
 * c) Any "$TD SENT ..." messages confirming data had been successfully uploaded
 *    to the satellite from the device.
 * 
 * @param client (WiFiClient): Client device.
 * 
 * @returns (char *): Pointer to static character array of up to `MAX_MESSAGE_LENGTH`.
 *  which exists within the `readData` function.
*/
char *readData(WiFiClient client);

/**
 * Function which calculates the checksum to append at the end of outgoing messages.
 * 
 * @param message (const char *): Zero-terminated message string.
 * @param length (size_t): Length of the message being sent.
 * 
 * @returns (uint8_t): 8-bit checksum value to append to the end of the message.
*/
uint8_t nmeaChecksum(const char *message, size_t length);

#endif /* MODEM_H */
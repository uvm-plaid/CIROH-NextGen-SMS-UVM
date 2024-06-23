/**
 * printing.h
 * 
 * Header file containing function declarations for console output.
 * 
 * Created as part of the UVM CIROH project.
 * 
 * Author: Jordan Bourdeau
 * Date Created: 6/23/24
*/

#ifndef PRINTING_H
#define PRINTING_H

#include <stdint.h>

/**
 * Function which prints the mac address of a device.
 * 
 * @param mac (uint8_t array): Array of bytes for the MAC address.
*/
void printMacAddress(uint8_t mac[]);

/**
 * Function printing the board's local IP and MAC address.
*/
void printWifiData();

/**
 * Function printing the following:
 *  - SWARM_SSID of the currently connected network
 *  - MAC address of currently connected router
 *  - Perceived signal strength
 *  - Encryption type
*/
void printCurrentNet();

#endif /* PRINTING_H */
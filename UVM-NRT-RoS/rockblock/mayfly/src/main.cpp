/*
	Blink
	Turns on an LED on for one second, then off for one second, repeatedly.

	Most Arduinos have an on-board LED you can control. On the Uno and
	Leonardo, it is attached to digital pin 13. On the Mayfly there are LEDs
	at pins 8 and 9. If you're unsure what pin the on-board LED is connected
	to on your Arduino model, check the documentation at http://www.arduino.cc

	This example code is in the public domain.

	modified 8 May 2014
	by Scott Fitzgerald
	modified 9 February 2018 for Mayfly
	by Beth Fisher
 */

/*#include "MFCC_RF.h"*/
#include "loraDevice.h"
#include "printing.h"
#include "satellite.h"

#include <Arduino.h>
#include <IridiumSBD.h>
#include <SdFat.h>
#include <SPI.h>
#include <TimeLib.h>
#include <Wire.h>

// Default chip select pin for Mayfly SD card
constexpr int chip_select = 12;
// Digital pin to enable 5V boost voltage for UART output
constexpr int boost_5v_pin = 22;
SdFat sd;
SdFile file;
constexpr uint8_t PERIPHERAL_ADDRESS = 8;  // Hardcoded address for I2C peripheral
constexpr uint8_t I2C_BUFFER_SIZE = 32;
static uint8_t I2C_BUFFER[I2C_BUFFER_SIZE + 1];

// Commented out to decrease compile times for now
/*Eloquent::ML::Port::RandomForest rf_clf;*/
/*const char *csv_name = "/mfccs_only.csv";*/
/*const uint32_t num_mfccs = 13;*/
/*static float X[num_mfccs];*/
/*static int label;*/
/*static char line_buffer[256];*/

void print_directory();

uint32_t setup_sd() {
	if (!sd.begin(SS, SPI_MODE0)) {
		Serial.println("SD card initialization failed!");
		return 1;
	}
	Serial.println("SD card initialized.");
	return 0;
}

// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin 8 as an output.
	// green LED on Mayfly logger
	pinMode(8, OUTPUT);
    pinMode(boost_5v_pin, OUTPUT);
    digitalWrite(boost_5v_pin, HIGH);

	Serial.begin(9600);
	Serial1.begin(19200);
	while (!Serial || !Serial1);
	delay(500);
	if (setup_sd()) {
		return;
    }

    // Start I2C
    Wire.begin();

	// Print all filenames
	/*print_directory();*/
}

/* Function which fills the I2C buffer by requesting as many bytes as it can
 * fit from the peripheral.
 * @returns (uint32_t): Number of bytes read.
 */
uint32_t fill_i2c_buffer() {
    // I2C requests from the feather and files all the packets it can
    Wire.requestFrom(PERIPHERAL_ADDRESS, I2C_BUFFER_SIZE);
    uint32_t i = 0;
    while (Wire.available()) {
        I2C_BUFFER[i++] = Wire.read();
    }
    I2C_BUFFER[i] = 0;
    return i;
}

void print_time(time_t t) {
    printing::dbgln("Timestamp:");
    printing::dbgln("%d-%d-%d %d:%d:%d", year(t), month(t), day(t), hour(t), minute(t), second(t));
}

void loop() {
    // TODO: Have this function write packets to satellite to be sent with backoff
    static uint8_t OUT_BUF[sizeof(LoraPacket)];
    LoraPacket packet;
    LoraPacket::SerDeStatus status = LoraPacket::SerDeStatus::Valid;
	while (true) {
        fill_i2c_buffer();

        sat::SatCode rc;
        uint32_t i = 0;
        tmElements_t time;
        time_t timestamp;
        packet = LoraPacket::deserialize(I2C_BUFFER, I2C_BUFFER_SIZE, i, status);
        if (status == LoraPacket::SerDeStatus::Valid) {
            i = 0;
            while (status == LoraPacket::SerDeStatus::Valid &&
                (status = packet.serialize(OUT_BUF, sizeof(OUT_BUF), i)) 
                    == LoraPacket::SerDeStatus::Valid) {
                if ((rc = sat::get_time(time)) == sat::SatCode::Okay) {
                    timestamp = makeTime(time);
                    print_time(timestamp);
                    sat::send_packet("hello", 5);
                    // For now - finish
                    while (1);
                } else {
                    printing::dbgln("Error getting satellite time.");
                }
            }
        }
        /*printing::dbgln("Status after attempting to serialize packet: %d", (int) status);*/
        delay(1000);
	}
}

void print_directory() {
	Serial.println("Files in root directory:");
	SdFile dir;

	// Open the root directory
	if (!dir.openRoot(&sd)) {
		Serial.println("Failed to open root directory.");
		return;
	}

	// List files in the directory
	dir.rewind();
	const int max_name_size = 25;
	char filename[max_name_size];
	while (true) {
		memset(filename, 0, max_name_size);
		SdFile entry;
		if (!entry.openNext(&dir, O_READ))
			break;

		// Print the filename
		Serial.print("Filename: ");
		entry.getName(filename, max_name_size);
		Serial.println(filename);

		entry.close();
	}

	dir.close();
}

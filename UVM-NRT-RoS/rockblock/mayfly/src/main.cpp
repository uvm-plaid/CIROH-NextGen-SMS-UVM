#include <Arduino.h>
#include <IridiumSBD.h>
#include <SdFat.h>
#include "Sodaq_DS3231.h"
#include <SPI.h>
#include <TimeLib.h>
#include <Wire.h>

/*#include "MFCC_RF.h"*/
#include "loraDevice.h"
#include "printing.h"
#include "RandomForestClassifier.h"
#include "satellite.h"

SdFat sd;
SdFile file;
constexpr int MAYFLY_SD_PIN = 12;
constexpr int MAYFLY_BOOST_5V_PIN = 22;
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
void print_time(time_t t);
int setup_sd();
int setup_rtc();
uint32_t fill_i2c_buffer();

// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin 8 as an output.
	// green LED on Mayfly logger
	pinMode(8, OUTPUT);
    pinMode(MAYFLY_BOOST_5V_PIN, OUTPUT);
    digitalWrite(MAYFLY_BOOST_5V_PIN, HIGH);

	Serial.begin(9600);
	Serial1.begin(19200);
	while (!Serial || !Serial1);
	delay(500);
	if (setup_sd() != 0) {
        printing::dbgln("Error setting up SD card.");
		return;
    }
    if (setup_rtc() != 0) {
        printing::dbgln("Error setting up RTC.");
		return;
    }

    // Start I2C
    Wire.begin();
}

void loop() {
    // TODO: Have this function write packets to satellite to be sent with backoff
    static uint8_t OUT_BUF[sizeof(LoraPacket)];
    LoraPacket packet;
    LoraPacket::SerDeStatus status = LoraPacket::SerDeStatus::Valid;
	while (true) {
        fill_i2c_buffer();
        uint32_t i = 0;
        packet = LoraPacket::deserialize(I2C_BUFFER, I2C_BUFFER_SIZE, i, status);
        if (status == LoraPacket::SerDeStatus::Valid) {
            i = 0;
            while (status == LoraPacket::SerDeStatus::Valid &&
                (status = packet.serialize(OUT_BUF, sizeof(OUT_BUF), i)) 
                    == LoraPacket::SerDeStatus::Valid) {
                sat::send_packet("hello", 5);
                while (1);
            }
        }
        delay(1000);
	}
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

int setup_sd() {
	if (!sd.begin(SS, SPI_MODE0)) {
		Serial.println("SD card initialization failed!");
		return -1;
	}
	Serial.println("SD card initialized.");
	return 0;
}

int setup_rtc() {
    DateTime dt;
    time_t tt;
    tmElements_t time_elements;

    dt = rtc.now();
    tt = dt.getEpoch();
    printing::dbgln("RTC Time (Before):");
    print_time(tt);

    while (sat::get_time(time_elements) != sat::SatCode::Okay) {
        printing::dbgln("Retrying to get satellite time.");
    }

    rtc.begin();
    // Adjust for difference in epoch (1970-01-01 -> 2000-01-01)
    tt = makeTime(time_elements) - 30 * SECS_PER_YEAR;
    dt = DateTime(tt);
    rtc.setDateTime(dt);
    dt = rtc.now();
    tt = dt.getEpoch();
    printing::dbgln("RTC Time (After):");
    print_time(tt);

    return 0;
}

void print_time(time_t t) {
    printing::dbgln("Timestamp:");
    printing::dbgln("%d-%d-%d %d:%d:%d", year(t), month(t), day(t), hour(t), minute(t), second(t));
}

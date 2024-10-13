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

#include "MFCC_RF.h"
#include "printing.h"

#include <Arduino.h>
#include <IridiumSBD.h>
#include <SDFat.h>
#include <SPI.h>

// Default chip select pin for Mayfly SD card
const int chip_select = 12;
SdFat sd;
SdFile file;

Eloquent::ML::Port::RandomForest rf_clf;
const char *csv_name = "/mfccs_only.csv";
const uint32_t num_mfccs = 13;
static float X[num_mfccs];
static int label;
static char line_buffer[256];

void print_directory();

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 8 as an output.
  // green LED on Mayfly logger
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
    ;
  delay(500);

  printing::dbgln("%.2f", 3.853085);

  // Initialize SD card
  if (!sd.begin(SS, SPI_MODE0)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Print all filenames
  print_directory();
}

void loop() {
  file.open(csv_name, FILE_READ);
  // Parse CSV file and compute accuracy metrics on whole dataset
  int n = file.fgets(line_buffer, sizeof(line_buffer));
  int num_rows = 0;
  int correct_predictions = 0;
  while (file.available()) {
    int n = file.fgets(line_buffer, sizeof(line_buffer));
    // Parse columns
    char *pch = strtok(line_buffer, ",");
    int column = 0;
    while (pch != nullptr) {
      if (column < num_mfccs) {
        X[column] = atof(pch);
      } else {
        label = atoi(pch);
      }
      pch = strtok(nullptr, ",");
      ++column;
    }

    // Make prediction
    int prediction = rf_clf.predict(X);
    if (prediction == label) {
      ++correct_predictions;
    }

    ++num_rows;
  }
  printing::dbgln("Got %d / %d predictions correct", correct_predictions,
                  num_rows);

  while (true) {
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

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

#include "RandomForestClassifier.h"
#include "SVC.h"
#include "printing.h"

Eloquent::ML::Port::RandomForest rf_clf;
Eloquent::ML::Port::SVM svm_clf;
float X[] = {1.2, 3.2, 4.2, 5.4};

#include <Arduino.h>
#include <CSV_Parser.h>
#include <IridiumSBD.h>
#include <SDFat.h>
#include <SPI.h>

// Default chip select pin for Mayfly
const int chip_select = 12;
SdFat sd;
SdFile file;

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

  // Testing ML Models
  printing::dbgln("RF Prediction: %d", rf_clf.predict(X));
  printing::dbgln("SVM Prediction: %d", svm_clf.predict(X));

  // Initialize SD card
  if (!sd.begin(SS, SPI_MODE0)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Print all filenames
  print_directory();
}

// the loop function runs over and over again forever
void loop() {
  /*printing::dbgln("Hello %s", "World!");*/
  /*digitalWrite(8, HIGH); // turn the LED on (HIGH is the voltage level)*/
  /*delay(1000);           // wait for a second*/
  /*digitalWrite(8, LOW);  // turn the LED off by making the voltage LOW*/
  delay(1000); // wait for a second
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

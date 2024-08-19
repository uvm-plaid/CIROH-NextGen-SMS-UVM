/* This code has been modified by Soheyl
The code records 10 seconds of sound and calculates the FFT
It also stores this data onto the SD card (Both the sound and FFT)
*/

#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>
#include "arduinoFFT.h"

TMRpcm audio;

int file_number = 0;
char filePrefixname[50] = "rec";
char wavExten[10] = ".wav";
char csvExten[10] = ".csv";
const int recordLed = 2;
const int mic_pin = A0;
const int sample_rate = 16000;
#define SD_CSPin 12

// FFT configuration
const uint16_t samples = 128; // Must be a power of 2
const float samplingFrequency = 8000; // Hz
unsigned int sampling_period_us;
unsigned long microseconds;
float vReal[samples];
float vImag[samples];
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency, true);

bool header_written = false;

// Delay function with serial log.
void wait_min(int seconds) {
  int count = 0;
  while (count < seconds) {
    Serial.print('.');
    delay(1000);
    count++;
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  pinMode(mic_pin, INPUT);
  pinMode(recordLed, OUTPUT);
  Serial.println("Loading... SD card");
  if (!SD.begin(SD_CSPin)) {
    Serial.println("An Error has occurred while mounting SD");
    while (1);
  }
  audio.CSPin = SD_CSPin;
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
}

void loop() {
  Serial.println("===================================================================");

  // Create new file names
  char fileSlNum[20] = "";
  itoa(file_number, fileSlNum, 10);

  char wavFileName[50] = "";
  strcat(wavFileName, filePrefixname);
  strcat(wavFileName, fileSlNum);
  strcat(wavFileName, wavExten);

  char csvFileName[50] = "";
  strcat(csvFileName, filePrefixname);
  strcat(csvFileName, fileSlNum);
  strcat(csvFileName, csvExten);

  Serial.print("New WAV File Name: ");
  Serial.println(wavFileName);

  // Record audio
  digitalWrite(recordLed, HIGH);
  audio.startRecording(wavFileName, sample_rate, mic_pin);
  Serial.println("Start recording...");
  wait_min(10); // Record for 10 seconds
  digitalWrite(recordLed, LOW);
  audio.stopRecording(wavFileName);
  Serial.println("Recording stopped");

  // Perform FFT
  Serial.println("Performing FFT...");
  microseconds = micros();
  for (int i = 0; i < samples; i++) {
    vReal[i] = analogRead(mic_pin);
    vImag[i] = 0;
    while (micros() - microseconds < sampling_period_us) {
      // Empty loop to wait for the correct sampling time
    }
    microseconds += sampling_period_us;
  }
  
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); // Weigh data
  FFT.compute(FFTDirection::Forward); // Compute FFT
  FFT.complexToMagnitude(); // Compute magnitudes

  // Save FFT results to CSV
  Serial.print("Saving FFT data to ");
  Serial.println(csvFileName);

  File csvFile = SD.open(csvFileName, FILE_WRITE);
  if (csvFile) {
    if (!header_written) {
      csvFile.println("Frequency(Hz),Magnitude");
      header_written = true;
    }

    for (int i = 0; i < (samples >> 1); i++) {
      float frequency = (i * 1.0 * samplingFrequency) / samples;
      csvFile.print(frequency, 6);
      csvFile.print(",");
      csvFile.println(vReal[i], 4);
    }

    csvFile.close();
    Serial.println("FFT data saved");
  } else {
    Serial.println("Error opening CSV file for writing");
  }

  file_number++;
  Serial.println("===================================================================");
}

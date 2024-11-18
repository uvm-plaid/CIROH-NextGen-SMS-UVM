#include "MFCC_RF.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include "arduinoFFT.h"

TMRpcm audio;

int file_number = 0;
char filePrefixname[50] = "rec";
char wavExten[10] = ".wav";
char csvExten[10] = ".csv";
const int recordLed = 2;
const int mic_pin = A0;
#define SD_CSPin 12
#define powerPin 5
bool isFirstRow = true;

// FFT configuration
const int num_fft = 50;
const uint16_t samples = 256; // Number of samples per FFT
const uint16_t overlap = samples / 2; // 50% overlap
const float samplingFrequency = 16000; // Sampling frequency
float vReal[samples];
float vImag[samples];
float overlap_sample[overlap];
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency, true);

// Mel filterbank configuration
const int numMelFilters = 10; // You can use 26-40 in real applications
float melFilterBank[numMelFilters][samples / 2]; // Filterbank

// DCT and MFCC configuration
const int numMFCC = 13;
float mfcc[numMFCC] = {0}; // MFCC coefficients
float X[numMFCC]; // Array to store MFCC values for prediction
char line_buffer[256]; // Buffer to read lines from CSV file

Eloquent::ML::Port::RandomForest rf_clf;


void createMelFilterBank();
void computeMFCC(float* magnitudes, float* mfcc);
void wait_seconds(int seconds);

// Helper function to compute log base 10
float log10(float x) {
  return log(x) / log(10);
}

// Function to convert Hz to Mel scale
float hzToMel(float freq) {
  return 1127.0 * log(1.0 + freq / 700.0);
}

// Function to convert Mel scale to Hz
float melToHz(float mel) {
  return 700.0 * (exp(mel / 1127.0) - 1.0);
}

// Create the Mel filterbank
void createMelFilterBank() {
  float lowerMel = hzToMel(300);
  float upperMel = hzToMel(8000);
  
  float melPoints[numMelFilters + 2];
  for (int i = 0; i < numMelFilters + 2; i++) {
    melPoints[i] = lowerMel + i * (upperMel - lowerMel) / (numMelFilters + 1);
  }

  float hzPoints[numMelFilters + 2];
  int binPoints[numMelFilters + 2];
  for (int i = 0; i < numMelFilters + 2; i++) {
    hzPoints[i] = melToHz(melPoints[i]);
    binPoints[i] = floor((samples + 1) * hzPoints[i] / samplingFrequency);
  }

  for (int m = 1; m <= numMelFilters; m++) {
    for (int k = 0; k < samples / 2; k++) {
      if (k < binPoints[m - 1]) {
        melFilterBank[m - 1][k] = 0;
      } else if (k < binPoints[m]) {
        melFilterBank[m - 1][k] = (float)(k - binPoints[m - 1]) / (binPoints[m] - binPoints[m - 1]);
      } else if (k < binPoints[m + 1]) {
        melFilterBank[m - 1][k] = (float)(binPoints[m + 1] - k) / (binPoints[m + 1] - binPoints[m]);
      } else {
        melFilterBank[m - 1][k] = 0;
      }
    }
  }
}

// Apply DCT to convert to MFCC
void computeMFCC(float* magnitudes, float* mfcc) {
  float melEnergies[numMelFilters] = {0};

  // Apply the Mel filterbank
  for (int m = 0; m < numMelFilters; m++) {
    for (int k = 0; k < samples / 2; k++) {
      melEnergies[m] += magnitudes[k] * melFilterBank[m][k];
    }
    melEnergies[m] = log10(melEnergies[m] + 1e-10); // Avoid log of zero
  }

  // Apply Discrete Cosine Transform (DCT) to melEnergies to get MFCCs
  for (int i = 0; i < numMFCC; i++) {
    mfcc[i] = 0;
    for (int m = 0; m < numMelFilters; m++) {
      mfcc[i] += melEnergies[m] * cos(i * (m + 0.5) * PI / numMelFilters);
    }
  }
}

// Delay function with serial log
void wait_seconds(int seconds) {
  int count = 0;
  while (count < seconds) {
    Serial.print('.');
    delay(1000);
    count++;
  }
  Serial.println();
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(mic_pin, INPUT);
  pinMode(recordLed, OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH); // Set the power pin high

  Serial.println("Loading... SD card");
  if (!SD.begin(SD_CSPin)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  audio.CSPin = SD_CSPin;
  delay(100);
  Serial.println("SD card initialized.");
}

void loop() {
  createMelFilterBank(); // Create Mel filterbank during setup
  delay(1000);
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
  audio.startRecording(wavFileName, samplingFrequency, mic_pin);
  Serial.println("Start recording...");
  wait_seconds(2); // Record for 2 seconds
  digitalWrite(recordLed, LOW);
  audio.stopRecording(wavFileName);
  Serial.println("Recording stopped");

  // Process multiple blocks of 1024 samples each and store FFT results
  File wavFile = SD.open(wavFileName);
  if (!wavFile) {
    Serial.println("Error opening .wav file");
    return;
  }

  // Process multiple blocks
  int block_number = 0;
  isFirstRow = true;
  while (wavFile.available() && block_number < num_fft) {
    Serial.print("Processing block ");
    Serial.println(block_number + 1);

    if (isFirstRow) {
      // Read a block 
      for (int i = 0; i < samples && wavFile.available(); i++) {
        vReal[i] = wavFile.read(); // Assuming 8-bit audio data
        vImag[i] = 0; // Set imaginary part to 0
      }
    } else {
      for (int i = 0; i < overlap; i++) {
        vReal[i] = overlap_sample[i]; // Copy the second half of the previous block
        vImag[i] = 0; // Reset imaginary part
      }

      // Read new samples to complete the current block
      for (int i = overlap; i < samples && wavFile.available(); i++) {
        vReal[i] = wavFile.read(); // Assuming 8-bit audio data
        vImag[i] = 0; // Set imaginary part to 0
      }
    }
    for (int i = 0; i < overlap; i++) {
      overlap_sample[i] = vReal[i + overlap]; // Copy the second half of the previous block
    }

    // Perform FFT on the current block
    FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); // Apply Hamming window
    FFT.compute(FFTDirection::Forward); // Compute FFT
    FFT.complexToMagnitude(); // Get magnitudes from FFT

    // Compute MFCC from FFT magnitudes
    computeMFCC(vReal, mfcc);

    // Save MFCCs to CSV in row format
    File csvFile = SD.open(csvFileName, FILE_WRITE);
    if (!csvFile) {
      Serial.println("Error opening CSV file for writing");
      return;
    }

    if (isFirstRow) {
      String headerRow = "";
      for (int i = 0; i < numMFCC; i++) {
        headerRow += ",MFCC" + String(i + 1);
      }
      csvFile.println(headerRow); // Write the header row
    }

    for (int j = 0; j < numMFCC; j++) {
      csvFile.print(mfcc[j], 6); // Write each MFCC value in row format
      csvFile.print(",");
    }
    csvFile.println();

    isFirstRow = false; // Set the flag to f  alse so that the header row is written only once
    csvFile.close();

    block_number++;
  }

  wavFile.close();

  // Open the CSV file and feed the MFCCs to the Random Forest model for prediction
  File file = SD.open(csvFileName, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  // Parse CSV file and make predictions on each row of MFCC data
  while (file.available()) {
    file.readBytesUntil('\n', line_buffer, sizeof(line_buffer));
    // Parse columns
    char *pch = strtok(line_buffer, ",");
    int column = 0;
    while (pch != nullptr) {
      if (column < numMFCC) {
        X[column] = atof(pch);
      }
      pch = strtok(nullptr, ",");
      ++column;
    }

    // Make prediction
    int prediction = rf_clf.predict(X);
    Serial.print("Prediction: Class");
    Serial.println(prediction);
  }

  file.close();

  file_number++;
  Serial.println("===================================================================");
}
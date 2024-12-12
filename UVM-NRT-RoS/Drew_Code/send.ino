#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>

TMRpcm audio;
const int mic_pin = A0;
const int sample_rate = 16000;
#define SD_CSPin 12

void wait_min(int seconds) {
  for (int i = 0; i < seconds; i++) {
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(mic_pin, INPUT);
  Serial.println("Initializing SD card...");
  
  if (!SD.begin(SD_CSPin)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  audio.CSPin = SD_CSPin;
}

void loop() {
  char file_name[] = "temp.wav";

  Serial.println("Recording...");
  audio.startRecording(file_name, sample_rate, mic_pin);
  wait_min(10); // Record for 10 seconds
  audio.stopRecording(file_name);
  Serial.println("Recording stopped");

  File audioFile = SD.open(file_name, FILE_READ);
  if (audioFile) {
    Serial.println("Sending audio file...");
    Serial.println("<start>"); // Signal the start of the file
    while (audioFile.available()) {
      Serial.write(audioFile.read());
    }
    Serial.println("\n<end>"); // Signal the end of the file
    audioFile.close();
    Serial.println("Audio file sent");
    SD.remove(file_name); // Delete file after sending
  } else {
    Serial.println("Failed to open audio file!");
  }
  delay(5000);
}

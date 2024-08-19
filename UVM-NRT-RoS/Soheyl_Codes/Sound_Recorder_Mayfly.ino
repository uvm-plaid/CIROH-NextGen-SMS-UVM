/*
    1. On Uno or non-mega boards uncomment the line #define buffSize 128
    2. Also uncomment #define ENABLE_RECORDING and #define BLOCK_COUNT 10000UL
*/

// The code has been modified by Soheyl
// Connect the microphone to pin A0 of Mayfly, then power the Mayfly on.
// The code will record the voice in .wav format into the SD card.
// The SD card must be about 8 gig to 16 gig.
// Each recording is 10 seconds. You can change this time.

#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>
TMRpcm audio;


int file_number = 0;
char filePrefixname[50] = "rec";
char exten[10] = ".wav";
const int recordLed = 2;
const int mic_pin = A0;
const int sample_rate = 16000;
#define SD_CSPin 12
// delay function for with serial log.
void wait_min(int seconds) {
  int count = 0;
  int secs = seconds ;
  while (1) {
    Serial.print('.');
    delay(1000);
    count++;
    if (count == secs) {
      count = 0;
      break;
    }
  }
  Serial.println();
  return ;
}

void setup() {
  Serial.begin(9600);
  //Sets up the pins
  pinMode(mic_pin, INPUT);
  pinMode(recordLed, OUTPUT);
  Serial.println("loading... SD card");
  if (!SD.begin(SD_CSPin)) {
    Serial.println("An Error has occurred while mounting SD");
  }
  while (!SD.begin(SD_CSPin)) 
  {
    Serial.print(".");
    delay(500);
  }
  audio.CSPin = SD_CSPin;
}
void loop() {
  Serial.println("===================================================================");
  char fileSlNum[20] = "";
  itoa(file_number, fileSlNum, 10);
  char file_name[50] = "";
  strcat(file_name, filePrefixname);
  strcat(file_name, fileSlNum);
  strcat(file_name, exten);
  Serial.print("New File Name: ");
  Serial.println(file_name);
  digitalWrite(recordLed, HIGH);
  audio.startRecording(file_name, sample_rate, mic_pin);
  Serial.println("startRecording ");
  wait_min(10); // Record for 10 seconds
  digitalWrite(recordLed, LOW);
  audio.stopRecording(file_name);
  Serial.println("stopRecording");
  file_number++;
  Serial.println("===================================================================");
}
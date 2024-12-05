/*
This sketch demonstrates recording of standard WAV files that can be played on any device that supports WAVs. The recording
uses a single ended input from any of the analog input pins. Uses AVCC (5V) reference currently.

Requirements:
Class 4 or 6 SD Card
Audio Input Device (Microphone, etc)
Arduino Uno,Nano, Mega, etc.

Steps:
1. Edit pcmConfig.h
    a: On Uno or non-mega boards, #define buffSize 128. May need to increase.
    b: Uncomment #define ENABLE_RECORDING and #define BLOCK_COUNT 10000UL

2. Usage is as below. See https://github.com/TMRh20/TMRpcm/wiki/Advanced-Features#wiki-recording-audio for
   additional informaiton.

Notes: Recording will not work in Multi Mode.
Performance is very dependant on SD write speed, and memory used.
Better performance may be seen using the SdFat library. See included example for usage.
Running the Arduino from a battery or filtered power supply will reduce noise.
*/

#include <SdFat.h>
#include <SPI.h>
#include <TMRpcm.h>
#include <pcmConfig.h>
//Uncomment this, the include above and in pcmConfig.h if using SdFat library
SdFat SD;

// #define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 12  //using digital pin 4 on arduino nano 328, can use other pins

TMRpcm audio;   // create an object for use in this sketch 

void setup() {
  
  // Enable 15 V output
  pinMode(22, OUTPUT);      // pin D22 is the enable line for the Mayfly's switched 3.3/5v power lines
  digitalWrite(22, HIGH);   // set this pin high and leave it on for the rest of the sketch
  delay(200);
  
  Serial.begin(57600);
  
  if (!SD.begin(SD_ChipSelectPin)) {  
    Serial.println("SD Failed");
    return;
  }else{
    Serial.println("SD OK"); 
  }
  // The audio library needs to know which CS pin to use for recording
  audio.CSPin = SD_ChipSelectPin;
}


void loop() {
  
    if(Serial.available()){                          //Send commands over serial to play
      switch(Serial.read()){
        case 'r':
          for (int i = 0; i < 1; ++i) {
            // update name for the file here
            String filename = "TestingSamples/metalking" + String(i) + ".wav";
            
            // Convert String to const char*
            const char* file = filename.c_str();

            // Here we pass in filename, sample rate, and pin with microphone output to start recording
            // Common options for sample rate are:
            //    16000 == 16 kHz
            //    44100 == 44.1 kHz
            audio.startRecording(file, 16000, A0);
            Serial.println("Recording " + String(i) + " started");

            // Sample length in ms
            delay(6000); 

            // Stop recording
            audio.stopRecording(file);
            Serial.println("Recording " + String(i) + " stopped");
          }
          Serial.println("done");
          break;    
        case 'R': audio.startRecording("test.wav",16000,A0,1); break;  //Record, but with passthrough to speaker.
        case 't': audio.startRecording("test.wav",16000,A0,2); break;  //Do not record. Output direct to speaker
        							       //Note: If samples are dropped before writing, it
        							       //      will not be heard in passthrough mode
        case 's': audio.stopRecording("file"); Serial.println("stop"); break;              //Stop recording
        case 'p': audio.play("test.wav"); break;                       //Play the recording 
        case '=': audio.volume(1); break;                              //Increase volume by 1. Does not affect recording
        case '-': audio.volume(0); break;                              //Decrease volume by 1. Does not affect recording
        case 'S': audio.stopPlayback(); break;                         //Stop all playback
      }
    }
}
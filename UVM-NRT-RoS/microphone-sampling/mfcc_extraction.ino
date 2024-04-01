// Source: https://www.instructables.com/Speech-Recognition-With-an-Arduino-Nano/

const int SAMPLE_LENGTH = 1000; // 1000ms
int *amplitudes;

const int AUDIO_IN = A0;
const int AMPLITUDES_LENGTH = SAMPLE_LENGTH*7; // Getting approximately 6600-6950 amplitudes/audio samples per second

void setup() {
  Serial.begin(57600);
  pinMode(22, OUTPUT);      // pin D22 is the enable line for the Mayfly's switched 3.3/5v power lines
  digitalWrite(22, HIGH);   // set this pin high and leave it on for the rest of the sketch
  delay(200);
  pinMode(A0, INPUT);      // pin D22 is the enable line for the Mayfly's switched 3.3/5v power lines
  amplitudes = new int[AMPLITUDES_LENGTH];
}
//------------------------------------------------------------------------------

int index;
const int WINDOW_LENGTH = 30; // 30 ms
void loop() {

  /*
  STEP 1: Calculate Raw Amplitudes from 1 second (1000ms) audio samples
  */
  // Start timer
  double t0 = millis();
  int amplitude = -1;
  index = 0;
  // Record Amplitudes
  while (millis()-t0 < SAMPLE_LENGTH){
    ++index;
    amplitude = analogRead(AUDIO_IN);
    amplitudes[index] = amplitude;
  }
  // Fill in remaining amplitude values with -1
  for(int i = index; i < AMPLITUDES_LENGTH; ++i){
    amplitudes[i] = 0;
  }
  // Print results
  Serial.print("Time to fill ");
  Serial.print(index);
  Serial.print("-length array: ");
  Serial.print(millis()-t0);
  Serial.println(" ms");
  /*
  STEP 2: Windowing
    Split frames into 30 ms audio "windows" with 50% overlap
  */
  
  

}

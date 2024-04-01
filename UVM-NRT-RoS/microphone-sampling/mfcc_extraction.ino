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

  /**********
  STEP 1: Calculate Raw Amplitudes from 1 second (1000ms) audio samples
  **********/
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
  Serial.print("-length array (ms): ");
  Serial.println(millis()-t0);
  Serial.print("Sample Rate (Hz): ");
  Serial.println(index);
  /**********
  STEP 2: Windowing
    Split frames into 30 ms audio "windows" with 50% overlap of each window
  **********/
  // 2a): Calculate number of windows needed (approximately 410-470 windows) and record index positions of these windows
  int n_windows = 1;
  int beg_index = 0;
  int beg_indexes[470];
  int index_pos;
  while(beg_index <= index-(WINDOW_LENGTH*(index/1000.0))){
    index_pos = n_windows-1;
    beg_indexes[index_pos] = beg_index;
    beg_index += (WINDOW_LENGTH*(index/1000.0))/2;
    ++n_windows;
  }
  Serial.print(n_windows);
  Serial.println(" windows");
  // 2b) Create 2d-array, windows, where rows are each window, and cols are amplitude values for windows
  int ROWS = n_windows;
  int COLS = WINDOW_LENGTH * (index/1000.0);
  int windows[ROWS][COLS];
  for (int row = 0; row < ROWS; ++row){
    for(int col = 0; col < COLS; ++col){
      int amplitude_value = amplitudes[beg_indexes[row]+col];
      windows[row][col] = amplitude_value;
    }
  }
}

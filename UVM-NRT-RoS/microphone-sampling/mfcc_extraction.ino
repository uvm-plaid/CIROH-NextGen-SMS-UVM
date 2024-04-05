// Source: https://www.instructables.com/Speech-Recognition-With-an-Arduino-Nano/

const short SAMPLE_LENGTH = 1000; // 1000ms
short *amplitudes;

const short AUDIO_IN = A0;
const short AMPLITUDES_LENGTH = SAMPLE_LENGTH*7; // Getting approximately 6600-6950 amplitudes/audio samples per second

short index;
const short WINDOW_LENGTH = 30; // 30 ms

double hamming_window(int n, int N) {
    return (0.54 - 0.46 * cos(2 * M_PI * n / (N - 1)));
}

void setup() {
  Serial.begin(115200);
  pinMode(22, OUTPUT);      // pin D22 is the enable line for the Mayfly's switched 3.3/5v power lines
  digitalWrite(22, HIGH);   // set this pin high and leave it on for the rest of the sketch
  pinMode(A0, INPUT);      // pin D22 is the enable line for the Mayfly's switched 3.3/5v power lines
  amplitudes = new int[AMPLITUDES_LENGTH];
}

void loop() {

  /**********
  STEP 1: Calculate Raw Amplitudes from 1 second (1000ms) audio samples
  **********/
  //calculate_amplitudes(*amplitudes);
  //Serial.println(amplitudes[5000]);
  // Start timer
  double t0 = millis();
  short amplitude = -1;
  index = 0;
  // Record Amplitudes
  while (millis()-t0 < SAMPLE_LENGTH){
    ++index;
    // Manually reduce the audio resolution to prevent system fault later on
    delay(1);
    amplitude = analogRead(AUDIO_IN);
    amplitudes[index] = amplitude;
  }
  // Fill in remaining amplitude values with -1
  for(short i = index; i < AMPLITUDES_LENGTH; ++i){
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
  short n_windows = 1;
  short beg_index = 0;
  short beg_indexes[470];
  short index_pos;
  while(beg_index <= index-(WINDOW_LENGTH*(index/1000.0))){
    index_pos = n_windows-1;
    beg_indexes[index_pos] = beg_index;
    beg_index += (WINDOW_LENGTH*(index/1000.0))/2;
    ++n_windows;
  }
  Serial.print(n_windows);
  Serial.println(" windows");
  // 2b) Create 2d-array, windows, where rows are each window, and cols are amplitude values for windows
  short ROWS = n_windows;
  short COLS = WINDOW_LENGTH * (static_cast<double>(index) / 1000.0);
  short windows[ROWS][COLS];
  
  short bad_readings = 0;
  for (short r = 0; r < ROWS; ++r){
    for(short c = 0; c < COLS; ++c){
      short amplitude_index = beg_indexes[r] + c;
      if (amplitude_index > index){
        amplitude_index = index;
      } else if (amplitude_index < 0){
        amplitude_index = 0;
      }
      short amplitude_value = amplitudes[amplitude_index];
      // Handle erroneous readings (all readings should be between 2^0-1 and 2^10 -1)
      if (amplitude_value > 1023){
        ++bad_readings;
        amplitude_value = 1023;
      }
      if (amplitude_value < 0){
        ++bad_readings;
        amplitude_value = 0;
      }
      windows[r][c] = amplitude_value;
    }
  }
  /**********
  STEP 3: Windowing Function
  Apply windowing function to each element of the array
  **********/
  for(short r = 0; r < ROWS; ++r){
    for(short c = 0; c < COLS; ++c){
      // Get the amplitude value
      short amplitude_value = windows[r][c];
      // Apply the Hamming window function to the amplitude value
      amplitude_value = static_cast<short>(amplitude_value * hamming_window(c, COLS));
      windows[r][c] = amplitude_value;
    }
  }
  /**********
  STEP 4: Fast Fourier Transform
  **********/
  

}

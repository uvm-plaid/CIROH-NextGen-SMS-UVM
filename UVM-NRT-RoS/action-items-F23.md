## Action items and progress reports for RoS and NRT comms research F23

## Week 1

### Chris

1. Reserve meeting space.
2. Set up git repo.
3. Arrange travel to [CIROH annual science meeting](https://ciroh.ua.edu/education-and-outreach/ciroh-annual-science-meeting/).

*Progress:*
1. Reserved E325 for Fall Semester.
2. Accomplished with this repo.
3. All set for Chris to attend meeting.

### Rachael

1. Coordinate first equipment purchase
2. Identify Temp/RH sensor
3. Begin investigation of Temp/RH sampling regime.

*Progress:*
1. Equipment purchase complete
2. Sensor identified
3. Investigation of Temp/RH sampling regime began on Wokwi emulator. Documentation found on measuring temperature using the Mayfly device.

### Quimby

1. Identify Lora shield for Mayfly
2. Identify Lora gateway
   
*Progress:*
1. Done, shared with Rachael for purchase.
2. Done, shared with Rachael for purchase.

### Spencer and Lars

1. Identify microphone 
2. Begin exploration of audio data format
3. Begin review of papers on ML for rain detection, and it's relation to our available audio data format. 
   
*Progress:*

1. Done, shared with Rachael for purchase.
2. Exploration of audio data format has been started.
3. Both Spencer have Lars have began reviewing the papers on ML for rain detection.

## Week 2

### Chris

1. Communicate with students as needed on topics.

*Progress:* Yes.

### Rachael

1. **(1st task): Set up GitHub branches for each institution. The current branch will become main.
2. Learn Bibtex and store research papers on UVM's git in Bibtex file
3. Continue investigation of Temp/RH sampling regime- how to program? (Code in Wokwi emulator; read article about programming Mayfly temperature readings and clock)

*Progress:* 

1. Yes, but with some planned modifications.
2. Future
3. Future

### Quimby

1. Verify comms between Lora gateway and shield(s), including multihop if possible (with Arduino MKR WAN 1310?)
2. Assists others as needed, read through further LoRa documentation (most tasks deferred until the devices come in).

*Progress:*

1. No progress, since no hardware
2. Progress.

### Spencer and Lars

1. Continue exploration of audio data format
2. Continue review of papers on ML for rain detection, and it's relation to our available audio data format.
3. Look into research done on how to get valuable data from the simple microhphones.
   
*Progress:* 

1,2,3: Enough progress to know we need to talk to signal processing expert.

## Week 3

### Chris

1. Communicate with signal processing expert.
2. Set up repos for other schools.

### Rachael

1. Order new mic parts, update inventory.
2. Learn Bibtex and store research papers on UVM's git in Bibtex file
3. Continue investigation of Temp/RH sampling regime- how to program? (Code in Wokwi emulator; read article about programming Mayfly temperature readings and clock) (May not have enough time this week).

### Quimby

1. Verify comms between Lora gateway and shield(s).

### Spencer and Lars

1. Identify mic parts.
2. Explore running ML models (esp. tensorflow lite) on arduino.

## Week 4

### Chris

*Progress:*

1. Yes; experts not needed. Use MFCCs, libra. 
2. Set up repos for other schools: done.

### Rachael

1. Learn Bibtex and store research papers on UVM's git in Bibtex file
2. Continue investigation of Temp/RH sampling regime- how to program? (Code in Wokwi emulator; read article about programming Mayfly temperature readings and clock) (May not have enough time this week).

*Progress:*

1. Order new mic parts, update inventory: done
2. Learn Bibtex and store research papers on UVM's git in Bibtex file
3. Continue investigation of Temp/RH sampling regime- how to program? (Code in Wokwi emulator; read article about programming Mayfly temperature readings and clock) (May not have enough time this week). : made some initial progress with programming.

### Quimby

1. Verify comms between Lora gateway and shield(s)

*Progress*

1. Verify comms between Lora gateway and shield(s): some progress. 

### Spencer and Lars

1. Test a model on arduino with microflow
2. Build and test microphone
3. Identify and begin working with MFCC extraction in librosa

Progress: 

1. Identify mic parts: done.
2. Explore running ML models (esp. tensorflow lite) on arduino: some progress- identified microflow as a version of tensorflow lite that will run on arduino. 
   
## Week 5

### Chris

*Progress:*

1. Yes; experts not needed. Use MFCCs, libra. 
2. Set up repos for other schools: done.

### Rachae

1. Figure out if we can connect arduinos.
2. Figure out if nano has ADC, and if RHT sensor requires ADC.
3. Continue investigation of Temp/RH sampling regime- how to program? (Code in Wokwi emulator; read article about programming Mayfly temperature readings and clock) (May not have enough time this week).
   
*Progress*

1. Learn Bibtex and store research papers on UVM's git in Bibtex file: done!
2. Continue investigation of Temp/RH sampling regime- how to program? (Code in Wokwi emulator; read article about programming Mayfly temperature readings and clock) (May not have enough time this week). : no progres.

### Quimby

1. Talk to Penni for remuneration on cords, buy a nano.
2. Start investigating mesh networking.
3. Investigate advantages of LoraWAN- do we need gateway?

*Progress* 

1. Verify comms between Lora gateway and shield(s) : verified

### Spencer and Lars

1. Communicate with Quimby about which nano.
2. Investigate any expected limitations of models on nano.
3. Build and test microphone.

*Progress*

1. Test a model on arduino with microflow: microflow too weak. TensorFlow Lite only works on nano.
2. Build and test microphone: no progress.
3. Identify and begin working with MFCC extraction in librosa: straightforward! both for MFCC and HF calculation.
   
## Week 6

### Chris

*Progress:*

1. Yes; experts not needed. Use MFCCs, libra. 
2. Set up repos for other schools: done.

### Rachael

1. Figure out how to sample temperature on arduino.
2. Figure out how to sample RH.
3. Define dewpoint computation for calculation from temp and RH.
4. Figure out how to control the control loop timing. 

*Progress* 

1. Figure out if we can connect arduinos: yes we can.
2. Figure out if nano has ADC, and if RHT sensor requires ADC: still figuring this.
3. Continue investigation of Temp/RH sampling regime- how to program? (Code in Wokwi emulator; read article about programming Mayfly temperature readings and clock): Figured out how to connect arduino to laptop.

### Quimby

1. Talk to Penni for remuneration on cords, buy a nano.
2. Begin to figure out low level detail of data comms and Lora protocol, whether it makes sense to use loramesh for collision avoidance, reliability. 

*Progress*

1. Talk to Penni for remuneration on cords, buy a nano: waiting for Penni.
2. Start investigating mesh networking.
3. Investigate advantages of LoraWAN- do we need gateway?

*Progress* 

1. Verify comms between Lora gateway and shield(s) : verified

### Spencer and Lars

1. Record sound from mic as .wav on arduino.
2. Investigate any expected limitations of models on nano: size and what activation functions.
3. Start to consider workflow, .wav to MFCC and HF extraction to tensorflow

*Progress*

1. Communicate with Quimby about which nano; figured out. 
2. Investigate any expected limitations of models on nano: started study
3. Build and test microphone: built, sound coming to computer. 

# How to run the data collecting code

## The Stucutre of the code is as follows 

send.ino is on the mayfly  
pass_file.py is on the raspberry pi
upload.php is on the waiting silk server
The mayfly should be connected to the pi via a USB connection

## File Structure

In the directory holding the pass_file.py, there should be a folder where audio files are temporaroly held, in the code this directory is made if it does not already exist and names it output

In the directory holding the upload.php, there sould be a folder where the final audio is held, in the file it expects that this folder is named "uploads"

## Variables to Change

The only variable that needs to be changed is the url variable in pass_file.py. This variable should be set to the silk server the files will be sent to. Run as it is now, it will be sent to Drew Jepsen's silk server

## How to Run

To run, just run the pass_file.py on the raspberry pi. This will run indefinetly. Additionally, a Cron job can be set to run the file in case the pi gets reset for some reason. The serial communication between the mayfly and the pi is the longest part of the process taking about a minute to send a .WAV file. Everything else is pretty much instant.

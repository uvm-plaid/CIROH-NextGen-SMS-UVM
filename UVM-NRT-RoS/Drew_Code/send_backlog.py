import os
from pass_file import send_wav_file
import time

# Use the same output directory and URL as in pass_file.py
OUTPUT_DIR = "output"
url = "https://cdjepsen.w3.uvm.edu/research/upload.php"

def send_backlog_files():
    """
    Sends all WAV files from output directory to remote server.
    Files are deleted after successful upload.
    """
    # Get list of all WAV files
    wav_files = [f for f in os.listdir(OUTPUT_DIR) if f.endswith('.WAV')]
    
    if not wav_files:
        print("No WAV files found in output directory.")
        return

    print(f"Found {len(wav_files)} WAV files to process.")

    # Process each file
    for filename in wav_files:
        file_path = os.path.join(OUTPUT_DIR, filename)
        print(f"\nProcessing: {filename}")
        
        response = send_wav_file(file_path, url)
        
if __name__ == "__main__":
    send_backlog_files()

#Drew Jepsen
import requests
import serial
import datetime
import os
import time


SERIAL_PORT = "/dev/ttyUSB0"  
BAUD_RATE = 9600
OUTPUT_DIR = "output"
url = "https://cdjepsen.w3.uvm.edu/research/upload.php"


import os
import requests

def send_wav_file(file_path, url):
    """
    Sends a .WAV file to a specified server URL using an HTTP POST request.

    Parameters:
        file_path (str): The path to the .WAV file to send.
        url (str): The server URL to send the file to.

    Returns:
        response: The server's response to the POST request.
    """
    try:
        with open(file_path, 'rb') as file:
            files = {'file': (file_path, file, 'audio/wav')}
            response = requests.post(url, files=files)

        if response.status_code == 200:
            print("File uploaded successfully.")
            os.remove(file_path)
        else:
            print(f"Failed to upload file. Status code: {response.status_code}")
            print(response.text)

        return response

    except Exception as e:
        print(f"An error occurred: {e}")



def is_precipitating():
    """
    Determines if there is any precipitation (rain, hail, or snow) in the Northern Vermont area using the Open-Meteo API.

    Returns:
        bool: True if there is precipitation, False otherwise.
    """
    latitude = 44.5  
    longitude = -72.7 
    
    #API endpoint
    url = "https://api.open-meteo.com/v1/forecast"

    # Query parameters
    params = {
        "latitude": latitude,
        "longitude": longitude,
        "current_weather": True
    }

    try:
        #API request
        response = requests.get(url, params=params)
        response.raise_for_status()  # Raise an HTTPError for bad responses (4xx or 5xx)
        data = response.json()
        current_weather = data.get("current_weather", {})
        
        # Check if there is precipitation
        weather_code = current_weather.get("weathercode", -1)
        precipitation_codes = [61, 63, 65, 66, 67, 71, 73, 75, 77, 80, 81, 82, 85, 86]  # Rain, snow, hail-related weather codes
        has_precipitation = weather_code in precipitation_codes

        if has_precipitation:
            print("It is precipitating")
        else:
            print("It is not precipitating")

        return has_precipitation

    except Exception as e:
        print(f"An error occurred while fetching the weather data: {e}")
        return False


def save_audio_file(audio_data):
    """
    Save the received audio data as a .WAV file.
    """
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    file_name = f"{OUTPUT_DIR}/{timestamp}.WAV"

    with open(file_name, "wb") as f:
        f.write(audio_data)

    print(f"Saved audio file: {file_name}")
    return file_name

def read_audio_file(ser):
    """
    Read audio file data from the serial connection between <start> and <end>.
    Assumes the raw .WAV data is on a single line following <start>.
    """
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()

        if line == "<start>":
            print("Start of audio file detected.")
            # Read the next line as the full .WAV data
            audio_line = ser.readline()
            return audio_line
        elif line == "<end>":
            print("<end> marker received.")
            break

    return None

def main():
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=30) as ser:
        print("Listening for audio data...")

        while True:
            try:
                audio_data = read_audio_file(ser)
                if audio_data and is_precipitating(): # Remove the is_precipitating() call when u want all recordings to pass
                    fname=save_audio_file(audio_data)
                    send_wav_file(f'{fname}',url)
                else:
                    print("Record Next File.")
            except Exception as e:
                print(f"Error: {e}")

if __name__ == "__main__":
    main()

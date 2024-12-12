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
    Determines if there is any precipitation (rain, hail, or snow)

    Returns:
        tuple: (bool, str)
            bool: True if there is precipitation, False otherwise.
            str: The type of precipitation (e.g., "rain", "snow", "hail"). Returns "none" if no precipitation.
    """
    latitude = 44.5  # Example latitude
    longitude = -72.7  # Example longitude

    # API endpoint for the NWS gridpoints
    point_url = f"https://api.weather.gov/points/{latitude},{longitude}"

    try:
        # Get the gridpoint details (forecast office and grid coordinates)
        point_response = requests.get(point_url)
        point_response.raise_for_status()
        point_data = point_response.json()

        # Construct the observations API URL
        observation_station_url = point_data["properties"]["observationStations"]
        stations_response = requests.get(observation_station_url)
        stations_response.raise_for_status()
        stations_data = stations_response.json()

        # Use the first available observation station
        first_station = stations_data["features"][0]["properties"]["stationIdentifier"]
        observation_url = f"https://api.weather.gov/stations/{first_station}/observations/latest"

        # Fetch the latest observation
        observation_response = requests.get(observation_url)
        observation_response.raise_for_status()
        observation_data = observation_response.json()

        # Check for precipitation in the latest observation
        text_description = observation_data["properties"].get("textDescription", "").lower()
        precipitation_types = ["rain", "snow", "hail", "drizzle"]

        for precip in precipitation_types:
            if precip in text_description:
                print(f"It is precipitating: {precip}")
                return True, precip

        print("It is not precipitating")
        return False, "none"

    except Exception as e:
        print(f"An error occurred while fetching the weather data: {e}")
        return False, "error"


def save_audio_file(audio_data,code):
    """
    Save the received audio data as a .WAV file.
    """
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    file_name = f"{OUTPUT_DIR}/{timestamp}-code{code}.WAV"

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
                precip=is_precipitating()
                if audio_data and precip[0]: # Comment out precip when u want all recordings to pass
                    fname=save_audio_file(audio_data,precip[1])
                    send_wav_file(f'{fname}',url)
                else:
                    print("Record Next File.")
            except Exception as e:
                print(f"Error: {e}")

if __name__ == "__main__":
    main()

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
    latitude = 44.5
    longitude = -72.7

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

        precipitation_mapping = {
            # Rain variations
            "heavy rain": "heavy_rain",
            "torrential": "heavy_rain",
            "downpour": "heavy_rain",
            "rain shower": "heavy_rain",
            "thunderstorm": "heavy_rain",
            
            "moderate rain": "medium_rain",
            "steady rain": "medium_rain",
            "periods of rain": "medium_rain",
            "occasional rain": "medium_rain",
            
            "light rain": "light_rain",
            "drizzle": "light_rain",
            "scattered rain": "light_rain",
            "few rain": "light_rain",
            "spotty rain": "light_rain",
            "intermittent rain": "light_rain",
            
            # Hail variations
            "large hail": "heavy_hail",
            "heavy hail": "heavy_hail",
            "severe hail": "heavy_hail",
            
            "hail": "light_hail",
            "small hail": "light_hail",
            "light hail": "light_hail",
            
            # Snow variations
            "heavy snow": "snow",
            "moderate snow": "snow",
            "light snow": "snow",
            "snow shower": "snow",
            "flurries": "snow",
            "wintry mix": "snow",
            "sleet": "snow",
            "freezing rain": "snow",
            "cloudy": "snow"
        }

        for pattern, label in precipitation_mapping.items():
            if pattern in text_description:
                print(f"Precipitation detected: {label}")
                return True, label

        print("No precipitation detected")
        return False, "no_precipitation"

    except Exception as e:
        print(f"An error occurred while fetching the weather data: {e}")
        return False, "error"


def save_audio_file(audio_data,code):
    """
    Save the received audio data as a .WAV file.
    """
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    file_name = f"{OUTPUT_DIR}/{timestamp}_code_{code}.WAV"

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
                is_precip, precip_type = is_precipitating()
                
                if audio_data and is_precip:  # Only save and send if precipitation is detected
                    fname = save_audio_file(audio_data, precip_type)
                    send_wav_file(fname, url)
                else:
                    print("Skipping recording - no precipitation detected.")
            except Exception as e:
                print(f"Error: {e}")

if __name__ == "__main__":
    main()

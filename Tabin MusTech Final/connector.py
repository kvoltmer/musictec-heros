#!/usr/bin/env python3

# connect to ableton with https://github.com/ideoforms/pylive

import logging
import requests
import time
from live import *

logging.basicConfig(
    format="%(asctime)s - %(levelname)s - %(message)s",
    level=logging.DEBUG 
)
logging.getLogger("live").setLevel(logging.INFO)

API_URL = "http://192.168.137.111:5000/api/gsr"  # Flask API URL

def reset_solo_states(tracks):
    """Unsolo all tracks."""
    for track in tracks:
        track.solo = False

def main():
    set = Set(scan=True)
    tracks = set.tracks

    # Track the current range to avoid unnecessary resets
    current_range = None

    while True:
        try:
            # Query the Flask API (reads API signal)
            response = requests.get(API_URL)
            response.raise_for_status()
            data = response.json()

            # Extract the value from the JSON response ?
            if "gsr_value" not in data:
                raise ValueError("Invalid API response format: 'gsr_value' key not found.")
            value = int(data["gsr_value"])

            # Determine the range and corresponding track to solo (tracks in Ableton that need soloed.. tracking ranges)
            if value <= 250:
                track_to_solo = 0
                range_info = "Value <= 250"
            elif 251 <= value <= 325:
                track_to_solo = 1
                range_info = "Value between 251 and 325"
            elif 326 <= value <= 400:
                track_to_solo = 2
                range_info = "Value between 326 and 400"
            elif 401 <= value <= 475:
                track_to_solo = 3
                range_info = "Value between 401 and 475"
            else:
                track_to_solo = 4
                range_info = "Value > 475"

            # Check if the range has changed (selects which track needs to be soloed)
            if current_range != range_info:
                # Log the change and solo the appropriate track
                logging.info(f"Range changed: {range_info}, Value: {value}")
                reset_solo_states(tracks)
                tracks[track_to_solo].solo = True
                current_range = range_info
            else:
                logging.debug(f"No range change: {range_info}, Value: {value}")

        except Exception as e:
            logging.error(f"Error querying API or processing response: {e}")

        # Query the API 5 times per second
        time.sleep(0.2)

if __name__ == "__main__":
    main()

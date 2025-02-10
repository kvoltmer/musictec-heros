# Tabin MusTech Final

This project integrates a Grove GSR (Galvanic Skin Response) sensor with a Raspberry Pi to measure stress levels and use the data to control tracks in Ableton Live. A Flask server on the Raspberry Pi collects GSR readings, which are then accessed by a Python script using pylive to manipulate Ableton Live's parameters in real time. By mapping stress levels to musical changes, the system creates a dynamic and interactive experience where physiological responses influence sound production.


## 1. GSR Sensor & Raspberry Pi

#### Requirements 
- Hardware:
    - Raspberry Pi B+
    - Grove Hat
    - Grove GSR Sensor
- Software:
    - Python 3+
    - Install the sensor dependencies as explained in this [guide](https://wiki.seeedstudio.com/Grove-GSR_Sensor/#play-with-raspberry-pi-with-grove-base-hat-for-raspberry-pi).
    - In addition, install Flask using `pip install flask` so that we can run a server on the Raspberry Pi.

#### Starting the Server
- In order to run the server run the command `python grove_gsr_sensor_server.py`.



## 2. Ableton Live Connection

#### Requirements
- Hardware:
    - Windows Laptop
- Software:
    - Python 3+
    - Install [pylive](https://github.com/ideoforms/pylive) so that we can interact with Ableton Live using python.
    - Ableton Live 11 ;)


#### Starting the Connector
- To run the code which will request information from the server and control Ableton Live:
    1. Ensure that the `API_URL` is correctly set at the top of `connector.py` to match the server's IP address.
    2. Run the command `python connector.py`.
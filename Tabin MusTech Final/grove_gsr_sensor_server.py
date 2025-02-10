from flask import Flask, jsonify
from grove.adc import ADC


class GroveGSRSensor:
    def __init__(self, channel):
        self.channel = channel
        self.adc = ADC()

    @property
    def GSR(self):
        return self.adc.read(self.channel)


# Flask application
app = Flask(__name__)

# Initialize the sensor (channel 0 by default)
sensor = GroveGSRSensor(0)


@app.route('/api/gsr', methods=['GET'])
def get_gsr():
    """
    Endpoint to fetch the GSR value.
    """
    return jsonify({"gsr_value": sensor.GSR})


if __name__ == '__main__':
    # Run the app
    app.run(host='0.0.0.0', port=5000)

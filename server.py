from flask import Flask
from flask import request

app = Flask(__name__)

@app.route('/', methods=['GET'])
def handle_sensor_data():
    humidity = request.args.get('humidity')
    temperature = request.args.get('temp')

    print(f"Humidity: {humidity}")
    print(f"Temperature: {temperature}")

    return f"Received humidity= {humidity}, temp= {temperature} "
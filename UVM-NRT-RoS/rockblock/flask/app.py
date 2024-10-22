
import json

from flask import Flask, request
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import Integer, Float
from constants import DATABASE_URI

DEBUG = True

app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = DATABASE_URI

db = SQLAlchemy(app)

# Need to import data model here so it properly creates
# DB tables prior to starting to serve requests
from model import *

@app.route("/ciroh/echo", methods=["GET"])
def index():
    return f"Received: {request.args}"

@app.route

@app.route("/ciroh/save", methods=["POST"])
def save():
    sensor_id = request.json.get("s", None)
    temperature = request.json.get("t", None)
    humidity = request.json.get("h", None)
    precipitation_prediction = request.json.get("p", None)
    intensity_prediction = request.json.get("i", None)
    entry = SensorReading(
        sensor_id=sensor_id,
        temperature=temperature,
        humidity=humidity,
        precipitation_prediction=precipitation_prediction,
        intensity_prediction=intensity_prediction
    )
    db.session.add(entry)
    db.session.commit()

    return f"Success!\ Records:\n{SensorReading.query.all()}"


if __name__ == "__main__":
    app.run(debug=DEBUG, port=5000)

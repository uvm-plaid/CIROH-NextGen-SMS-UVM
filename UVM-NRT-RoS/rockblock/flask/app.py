
import json

from flask import Flask, request
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import Boolean, Float, Integer

from constants import DB_CONFIG, SECRET_KEY, SSL_ARGS

DEBUG = True

app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = DB_CONFIG
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False

app.secret_key = SECRET_KEY
db = SQLAlchemy(app)


class SensorReading(db.Model):
    reading_id = db.Column(Integer, primary_key=True)
    sensor_id = db.Column(Integer, nullable=False)
    # Readings & predictions go here
    temperature = db.Column(Float)
    humidity = db.Column(Float)
    precipitation_prediction = db.Column(Integer)
    intensity_prediction = db.Column(Integer)
    # ...


@app.route("/save", methods=["GET"])
def save():
    packet = json.loads(request.args.get("packet", "{}"))
    sensor_id = packet.get("s", None)
    temperature = packet.get("t", None)
    humidity = packet.get("h", None)
    precipitation_prediction = packet.get("p", None)
    intensity_prediction = packet.get("i", None)
    entry = SensorReading(
        sensor_id=sensor_id,
        temperature=temperature,
        humidity=humidity,
        precipitation_prediction=precipitation_prediction,
        intensity_prediction=intensity_prediction
    )
    db.session.add(entry)
    db.session.commit()

    return "Success!"


if __name__ == "__main__":
    with app.app_context():
        db.create_all()
    app.run(debug=DEBUG, port=5000)

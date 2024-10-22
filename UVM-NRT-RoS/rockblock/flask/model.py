"""
model.py

Data model for the Flask app.
Imports the app and database instances then creates
all the tables once ORM class definitions have been
made.
"""

from sqlalchemy import Integer, Float, Boolean
from app import app, db


class SensorReading(db.Model):
    reading_id = db.Column(Integer, primary_key=True)
    sensor_id = db.Column(Integer, nullable=False)
    # Readings & predictions go here
    temperature = db.Column(Float)
    humidity = db.Column(Float)
    precipitation_prediction = db.Column(Integer)
    intensity_prediction = db.Column(Integer)
    # ...

with app.app_context():
    db.create_all()

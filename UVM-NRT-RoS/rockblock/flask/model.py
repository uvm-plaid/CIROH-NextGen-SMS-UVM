"""
model.py

Data model for the Flask app.
Imports the app and database instances then creates
all the tables once ORM class definitions have been
made.
"""

from app import app, db

from sqlalchemy import Float, Integer, Text
from sqlalchemy.inspection import inspect
from typing import List, Literal

db.init_app(app)

# Mixins to simplify common operations on model classes

class Deletable:
    def delete(self) -> None:
        db.session.delete(self)


class Serializable:

    def serialize(self):
        return {
            c: getattr(self, c) 
            for c in inspect(self).attrs.keys() 
            if not c.startswith("_")
        }

    @staticmethod
    def serialize_list(objects: List[Literal['Serializable']]):
        return [item.serialize() for item in objects]

# Actual data model classes corresponding to tables

# Temporary table to save raw output to
class RawData(db.Model, Deletable, Serializable):
    id = db.Column(Integer, primary_key=True)
    imei = db.Column(Text)
    timestamp = db.Column(Text)
    data = db.Column(Text)

class SensorReading(db.Model, Deletable, Serializable):
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

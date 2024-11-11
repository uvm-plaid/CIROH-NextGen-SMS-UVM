"""
Flask app to receive Cloudloop data packets, parse the JSON
encoding of the Cloudloop packet, and also parse the format
of the data packet sent as JSON within the message coming
from the ROCKBLOCK (max 340 bytes).

Author: Jordan Bourdeau
"""


from flask import Flask, request
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import Integer, Float
from constants import DATABASE_URI, LOGGER

from parse import CloudloopPacket

DEBUG = True

app = Flask(__name__)

app.config["SQLALCHEMY_DATABASE_URI"] = DATABASE_URI

db = SQLAlchemy()

# Need to import data model here so it properly creates
# DB tables prior to starting to serve requests
from model import *

@app.route("/ciroh/echo", methods=["GET"])
def index():
    return f"Received: {request.args}"


@app.route("/ciroh/save_packet", methods=["POST", "GET"])
def save_packet():
    packet = CloudloopPacket(request.json)

    # Test entries for now which just save raw data
    entry = RawData(
        imei=packet.imei,
        timestamp=packet.sbd_received_at.isoformat(),
        data=packet.sbd_message,
    )
    db.session.add(entry)
    db.session.commit()

    return "Success!"

@app.route("/ciroh/add_record", methods=["GET"])
def add_record():
    entry = RawData(
        imei=1,
        timestamp=datetime.now(),
        data="Hello world",
    )

    db.session.add(entry)
    db.session.commit()

    return "Record added"

@app.route("/ciroh/last_record", methods=["GET"])
def last_record():
    entry = db.session.query(RawData).order_by(RawData.id.desc()).first()
    return str(entry.serialize())


if __name__ == "__main__":
    app.run(debug=DEBUG, port=5000)

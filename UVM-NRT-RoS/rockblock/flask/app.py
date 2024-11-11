"""
Flask app to receive Cloudloop data packets, parse the JSON
encoding of the Cloudloop packet, and also parse the format
of the data packet sent as JSON within the message coming
from the ROCKBLOCK (max 340 bytes).

Author: Jordan Bourdeau
"""

from constants import DATABASE_URI, FILE_DIRECTORY, LOGGER
from parse import CloudloopPacket

from datetime import datetime
import os
from flask import Flask, redirect, request
from flask_sqlalchemy import SQLAlchemy


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


@app.route("/ciroh/files", methods=["GET"])
def files():
    prefix, path = FILE_DIRECTORY.split("/www-root/")
    hostname = os.path.basename(prefix)
    return redirect(f"https://{hostname}.w3.uvm.edu/{path}")


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


@app.route("/ciroh/last_record", methods=["GET"])
def last_record():
    entry = db.session.query(RawData).order_by(RawData.id.desc()).first()
    return str(entry.serialize())


@app.route("/ciroh/save_file", methods=["POST"])
def save_file():
    files = request.files
    for _, stream in files.items():
        filename = os.path.basename(stream.filename)
        filename = str(datetime.now()) + " " + filename
        path = os.path.join(FILE_DIRECTORY, filename)
        with open(path, "wb") as outfile:
            outfile.write(stream.read())

    return "Ok"


if __name__ == "__main__":
    app.run(debug=DEBUG, port=5000)

"""
Script used to startup the Flask app.

Author: Jordan Bourdeau
"""

from constants import FILE_DIRECTORY

import os

os.makedirs(FILE_DIRECTORY, exist_ok=True)

from app import app as application


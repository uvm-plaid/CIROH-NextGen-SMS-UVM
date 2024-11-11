"""
Module containing constants

Author: Jordan Bourdeau
"""
from logger import Logger
import os

BASE_DIRECTORY = os.path.dirname(os.path.realpath(__file__))
DATABASE_FILE = "database.db"
DATABASE_URI = "sqlite:////" + os.path.join(BASE_DIRECTORY, DATABASE_FILE)

LOGGER = Logger()


"""
Python class for debugging by writing logs to an output file on the server.

Author: Jordan Bourdeau
"""

from typing import Any

class Logger:
    def __init__(self, logfile: str = "logs.txt"):
        self.logfile = logfile

    def write(self, value: Any):
        with open(self.logfile, "a") as outfile:
            outfile.write(str(value))

    def clear(self):
        with open(self.logfile, "w") as outfile:
            pass

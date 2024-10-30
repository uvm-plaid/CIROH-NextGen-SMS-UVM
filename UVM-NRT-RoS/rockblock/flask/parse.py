"""
Module for parsing incoming messages (From Cloudloop and for the JSON format
sensor data comes in).

Author: Jordan Bourdeau
"""

import base64
from datetime import datetime
import json
from typing import Dict
import zoneinfo as zi

class CloudloopPacket:
    IRIDIUM_TZ = zi.ZoneInfo("Etc/GMT")
    def __init__(self, data: Dict, local_tz: str = "US/Eastern"):
        self.data = data
        self.local_tz = zi.ZoneInfo(local_tz)

    @property
    def id(self) -> str:
        return self.data.get("id", "")

    @property
    def received_at(self) -> datetime:
        keys = ["year", "month", "day", "hour", "minute", "second"]
        iridium_timestamp = datetime(
            *(self.data.get(k, 0) for k in keys),
            tzinfo=self.IRIDIUM_TZ,
        )
        return iridium_timestamp.astimezone(tz=self.local_tz)

    @property
    def account_id(self) -> str:
        return self.data.get("accountId", "")

    @property
    def subscriber(self) -> Dict:
        return self.data.get("subscriber", {})

    @property
    def hardware(self) -> Dict:
        return self.data.get("hardware", {})

    @property
    def identifer(self) -> str:
        return self.data.get("identifier", "")

    @property
    def thing_id(self) -> str:
        return self.data.get("thingId", "")

    @property
    def sbd_received_at(self) -> datetime:
        sbd_entry = self.data.get("sbd", {})
        sbd_session_time = sbd_entry.get("sessionAt", {})
        keys = ["year", "month", "day", "hour", "minute", "second"]
        iridium_timestamp = datetime(
            *(sbd_session_time.get(k, 0) for k in keys),
            tzinfo=self.IRIDIUM_TZ,
        )
        return iridium_timestamp.astimezone(tz=self.local_tz)

    @property
    def sbd_message(self) -> str:
        sbd_entry = self.data.get("sbd", {})
        sbd_message = sbd_entry.get("message", "")
        return str(base64.b64decode(sbd_message))

    @property
    def imei(self) -> str:
        return self.data.get("sbd", {}).get("imei", "")

    @property
    def cdr_reference(self) -> str:
        return self.data.get("sbd", {}).get("cdrReference", "")

    # MO = Mobile Originated (from from device)
    # MT = Mobile Terminated (message to device)
    # SN = Sequence Number
    @property
    def sbd_mo_sn(self) -> int:
        return self.data.get("sbd", {}).get("momsn", -1)

    @property
    def sbd_mt_sn(self) -> int:
        return self.data.get("sbd", {}).get("mtmsn", -1)

    @property
    def sbd_signature(self) -> str:
        return self.data.get("sbd", {}).get("signature", "")

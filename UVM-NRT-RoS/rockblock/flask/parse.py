"""
Module for parsing incoming messages (From Cloudloop and for the JSON format
sensor data comes in).

Author: Jordan Bourdeau
"""

import base64
from constants import LOGGER
from datetime import datetime
import json
from typing import Any, Dict
import zoneinfo as zi

class CloudloopPacket:
    IRIDIUM_TZ = zi.ZoneInfo("Etc/GMT")
    def __init__(self, data: Dict, local_tz: str = "US/Eastern"):
        self.data = data
        LOGGER.write(data)
        self.local_tz = zi.ZoneInfo(local_tz)

    @property
    def id(self) -> str:
        return self.data.get("id", "")

    @property
    def identity(self) -> Dict[str, Any]:
        return self.data.get("identity", {})

    @property
    def sbd(self) -> Dict[str, Any]:
        return self.data.get("sbd", {})

    @property
    def received_at(self) -> datetime:
        keys = ["year", "month", "day", "hour", "minute", "second"]
        received_at = self.data.get("receivedAt", {})
        iridium_timestamp = datetime(
            *[received_at.get(k, 0) for k in keys],
            tzinfo=self.IRIDIUM_TZ,
        )
        LOGGER.write(iridium_timestamp)
        return iridium_timestamp.astimezone(tz=self.local_tz)

    @property
    def account_id(self) -> str:
        return self.identity.get("accountId", "")

    @property
    def subscriber(self) -> Dict:
        return self.identity.get("subscriber", {})

    @property
    def hardware(self) -> Dict:
        return self.identity.get("hardware", {})

    @property
    def identifier(self) -> str:
        return self.identity.get("identifier", "")

    @property
    def thing_id(self) -> str:
        return self.identity.get("thingId", "")

    @property
    def sbd_received_at(self) -> datetime:
        sbd_session_time = self.sbd.get("sessionAt", {})
        keys = ["year", "month", "day", "hour", "minute", "second"]
        iridium_timestamp = datetime(
            *(sbd_session_time.get(k, 0) for k in keys),
            tzinfo=self.IRIDIUM_TZ,
        )
        return iridium_timestamp.astimezone(tz=self.local_tz)

    @property
    def sbd_message(self) -> str:
        sbd_message = self.data.get("message", "")
        return base64.b64decode(sbd_message).decode("utf-8")

    @property
    def imei(self) -> str:
        return self.sbd.get("imei", "")

    @property
    def cdr_reference(self) -> str:
        return self.sbd.get("cdrReference", "")

    # MO = Mobile Originated (from from device)
    # MT = Mobile Terminated (message to device)
    # SN = Sequence Number
    @property
    def sbd_mo_sn(self) -> int:
        return self.sbd.get("momsn", -1)

    @property
    def sbd_mt_sn(self) -> int:
        return self.sbd.get("mtmsn", -1)

    @property
    def sbd_signature(self) -> str:
        return self.sbd.get("signature", "")

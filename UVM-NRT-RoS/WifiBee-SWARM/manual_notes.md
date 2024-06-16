Questions:
* What is wifi shielding?


Notes:
* 1kbps data rate
* Maximum packet size of 192 bytes
* 3.3V serial data interface
* 3.7s transmission length when sending packet to space
* 0.4s rest required before seconding another transmission

Power On/Off:
* **$PO*1F** used to power modem down
    * Can be externally switched on/off with user-supplied load switch on VDD power rail.
    * Should wait 1s before restoring power to modem
    * Restart by removing power if it does not respond to software commands

Serial Data Interface:
* UART 3-wire
    * Serial Rx
    * Serial Tx
    * Ground
* 115,200 baud
* 8 data bits
* No parity
* 1 stop bit
* No flow control

> Messages, data, and settings are stored in non-volatile memory

Communications:
* Serial Debug Header
    * Upgrade Modem hardware
    * Needs to be in the design to provide serial acces to the modem

Antenna Debugging:
* **$RT 1*17** command
    * Rapidly blinks LED

Software Interface:
* Messages are NMEA formatted sentences

NMEA Structure:

$CMD | (Arguments) | *xx (Checksum) | \n
* Starts with $
* Ends with 2 hex character checkum of the characters between & and *xx
* Bad checksums will produce `$CMD? ERR,CMD_NMEACHECKSUMFAIL*21`
* $ isn't used within commands, but can be used to reset the receiving state machine
* \* may occur within a command
* Once $ is received all subsequent characters must occur within 250 milliseconds of the previous character

Responses:
* `OK` confirms input parameters have been updated
* `ERR` response indicates command could not be parsed. Comes with error type.

$<cmd> ERR,<error_type>*xx


CMD_BADPARAM
Unrecognizable parameter after command

CMD_BADPARAMLENGTH
A parameter has an incorrect length

CMD_BADPARAMVALUE
A parameter has a value that is out of range

CMD_INVALIDCHAR
A parameter has an invalid character

CMD_NOTIMPLEMENTED
The command is not recognized as valid

CMD_PARAMMISSING
A required parameter is missing

CMD_PARAMDUPLICATE
A parameter has been duplicated

Messages:

**Unsolicited Messages:** 
* Status messages
* Data/time
* GPS information
* Notifications user data was received

**Command Responses:**
* Responses to input commands
* Notifications that user data has been sent or settings were updated

Boot:
* Ignore any characters received during startup until the following NMEA sentence:
    * `$M138 BOOT,RUNNING*2a`
* GPS will initialize after modem boot. Wait until this sequence:
    * `$M138 DATETIME*56`
    * Required after GPS cold start (fully powered off or in sleep mode for >4 hours) - takes ~30s

**Application ID:** Messages sent to or from the Hive have an ID associated with them.
* Arbitrary number from 0 to 64,999

```
uint8_t nmeaChecksum (const char *sz, size_t len) {
    size_t i = 0; 
    uint8_t cs;

    if (sz [0] == '$') 
        i++;

    for (cs = 0; (i < len) && sz [i]; i++) 
        cs ^= ((uint8_t) sz [i]);

    return cs; 
}
```

**Table of Commands:** (On page 38 of API manual)

* Outgoing message queue of up to 1024 unsent messages

# Notes

Jordan Bourdeau

8/22/24

## Takeaways

- Mobile Originated (MO) and Mobile Terminated (MT) messages allow
  two-way messages to be sent to/from the modem. - Sounds very useful for commanding network remotely
- Max packet size with header ad checksum is 340 bytes
  - Merge multiple messages into a single frame when sending
- Max packet sice of a received message is 270 bytes
- Sleep mode allows drastic reduction of passive power draw
- 24/7 satellite coverage from 66 Iridium satellite constellation

### Iridium SBD Library

- IridiumSBD uses a non-blocking retry strategy with a callback mechanism.
  - The callback is global and gets called repeatedly while the library waits for long operations to complete.
  - Non-reentrant callback - cannot call IridiumSBD methods from within
  - Can prematurely exit by returning false from the callback
- Can set power profile to limit retry attempt frequency which results in fewer recharges to the
  super capacity which supplies power to the Iridium device.
- Interface is transactional with paired send/receives but it shouldn't be an issue to
  do fire-and-forget if we never need to access certain messages

### Iridium Satellites

- There is a constellation of 66 Iridium satellites with a layout such that
  there is always at least one satellite accessible from any location on Earth at
  a time
- "Importantly, Iridium operates in the microwave L-band, which means that successful data transmission relies on having an unobstructed line-of-sight view of a satellite. Satellites travel quite quickly - from horizon to horizon in under 10 minutes - so with sensible applicaTon design, it is possible to transmit data, even with a heavily obstructed view of the sky."
  - Iridium Development Guide

### Rockblock 9603

- Comes with serial and UART connections
- After initial ~10s charging period power consumption is around 40mA @5V.
  Switching to sleep mode cuts this doesn to 20uA
  _ Reference power draw numbers in [9]
  _ Try transmitting every hour and queue messages on Arduino?
- TX and RX lines are relative from an attached arduino device.
  (e.g., TX is transmitting to the RockBLOCK, not from it)
- Messages must be terminated with a carriage return
- Needs at least 2 seconds after being turned off before turning back on
- Should provide an AT flush memory command before turning off (AT\*F)
- Network available output can be used to know when the transceiver has visiblity of
  satellite network (probably not useful when stationary, since we can rely
  on their always being an available satellite)

## References

1. [RockBLOCK Docs](https://docs.rockblock.rock7.com/docs/getting-started)
2. [Iridium SBD Docs](http://arduiniana.org/libraries/iridiumsbd/)
3. [Iridium SBD Source](https://github.com/mikalhart/IridiumSBD/tree/master)
4. [RockBLOCK Development Guide](https://cdn.sparkfun.com/assets/6/d/4/c/a/RockBLOCK-9603-Developers-Guide_1.pdf)
5. [Iridium Satellite Map](https://iridiumwhere.com)
6. [Iridium SBD Development Guide](https://fccid.io/document.php?id=2370311)
7. [Iridium ISU AT Command Reference](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.beamcommunications.com/document/328-iridium-isu-at-command-reference-v5&ved=2ahUKEwiRx6jmmImIAxVEMlkFHSY8Lq0QFnoECBwQAQ&usg=AOvVaw3JcNmRmEgJsIaUPvivEPXI)
8. [RockBLOCK Startup Guide](https://learn.adafruit.com/using-the-rockblock-iridium-modem?view=all)
9. [RockBLOCK Power Consumption](https://docs.rockblock.rock7.com/docs/power-consumption-guidance#:~:text=In%20sleep%20mode%2C%20all%20the,state%20ready%20for%20immediate%20use.)

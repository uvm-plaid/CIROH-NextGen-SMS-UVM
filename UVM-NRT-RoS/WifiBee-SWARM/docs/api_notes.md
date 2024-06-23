# API Notes

| Revision | Date       | Author          |
|----------|------------|-----------------|
|    1     |   6/23/24  | Jordan Bourdeau |


## Links

* [Home](../README.md)
* [Manual Notes](manual_notes.md)

Notes:
* Can send packets of 192 bytes, should design a way to consolidate as much information as possible into each byte
* Will likely need to configure the modem to wake up upon serial data being sent, which is when gateway node receive data via LoRa

Goals:
* Wrapping modem commands with a higher level API
    * Intaking data from nodes in the field, wrapping them into packets, and sending them via REST API
    * Receiving and unravelling packet data from the REST API

Questions:
* How does data get to the swarm device?
    * Via gateway node (Giga?) which receives packets that get routed through nodes in the network
* How is data transmitted from the Swarm device out?
    * REST API provided by Swarm 
* What is the required API?
* What calls/functionality need to be implemented?
    * read/write (send/receive) calls
* What are the packets which will be coming in through LoRaMESH?
* How many times a day does the satellite pass overhead?
    * Can store up to 1024 unsent messages, what will those messages hold?
    * Max packet length of 196 bytes (really 196 characters)


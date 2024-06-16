# Packet Data Format

| Revision | Date       | Author          |
|----------|------------|-----------------|
|    1     |   6/16/24  | Jordan Bourdeau |


## Collected Data

Data collected in the field will consist of sensor readings and the results of machine learning algorithms ran on the devices. Generally, these will be in the form of 1-4 byte values per sensor and algorithm which is running.
Date packets will generally be sent once per hour.

## Sensor Node Packet Format

There are three main components required for the sensor node packet format:

1. The packet is traceable to the location it came from. 
2. The packet is traceable to when it came from.
3. Collected data getts included in the packet.

For the time being, packets are encoded in JSON text format. While this is not the most space efficient encoding, the quantity of data being transmitted is small per packet and this minimizes the amount of complexity in the transfer of data.

The suggested packet structure is as follows:

```
{  
    // Packet identifiers
    "id": <site number>, // 8-bit integer mapping site locations to latitude/longitude in external database table
    "time": <timestamp>, // 32-bit signed integer representing UNIX timestamp when data was collected
    
    // Field values
    // Mappings are used for space efficiency. Human-readable names are stored in external database table
    "1": <value>, // variable size value
    ...
    "N": <value>,
}
```

Packet Sizes:

* 2 bytes for the braces {}
* 4 bytes per field for the quotations, colon, and comma (no extra whitespace is used in transmission)
* 7 bytes (2 bytes for 'id', 1 byte for the value, and 4 bytes per field) for the first field
* 12 bytes (4 bytes for 'time', 4 bytes for the value, and 4 bytes per field) for the second field
* 1 byte per sensor label
* 1-4 bytes per sensor reading

Bare packet with no timestamp would be 21 bytes. 
* 2 bytes for {}
* 7 bytes for id field
* 12 bytes for timestamp field

A sample packet with 10 values each of 4 bytes would have
* 21 bytes for bare timestamp
* 10 * 1 = 10 total bytes for the labels from each field
* 4 * 10 = 40 total bytes for the values from each field

= 71 bytes per packet

### Potential Ideas
* If space becomes an issue, a more compact/compressed binary format could be used instead of JSON

## Swarm Packet Format

The Swarm device is limited to sending 192-byte packets at a rate of 1 kbps (kilobits per second).
This is equivalent to 125 bytes/second. There are around 3-5, 9-10 minute windows per day when a satellite
flys overhead where the Swarm device is able to send data.

For the time being, the Giga will just naively send packets as they come in, without attempting to package them into a more condensed form. If throughput becomes an issue, a more sophisticated scheme of creating packets will be used.

### Potential Ideas:
* Specialize packets to each sensor type

```
// E.g. Packets are built up for each field. Create a new packet once there is no more room in the existing packet.
{
    "id": [0, 1, 2, ...],
    "timestamp": [230492385, 21832938, 23483567, ...],
    "N": [0, 1, 0, ...],
}
```

## Reconstructing Packets

With the current scheme, reconstructing packets is trivial and would just consist of parsing the JSON and writing it to a database.
This project connects feathers to each other using a time-division multiplexing scheme.

# Definitions
* **Frame**: A period of time during which every device has a window of time to communicate.
* **Node**: Subdivision of time within a frame where a single device has an opportunity to communicate.

# Open Questions
* **Reliability**: Should nodes reply an acknowledgement when they receive a packet from another node? If so, should that be done in the same window or a different one?
    * Related: Should same packet be resent multiple times? Right now only sending once, but it's easier to spam if that makes more sense to do.
    * My take: Yes and yes. However, typically these protocols involve one sender and one receiver, so having a back-and-forth communication makes me a little nervous.
* **Number of nodes**: No idea how many feathers we'll need. May need up to one node per recipient per feather that wants to communicate.
* **Duration of Node**: How long should each node have to communicate? Right now it's 200ms, based on nothing.
* **Time protocol**: Current protocol, using `millis()`, only allows uptime of 49.7 days before a crash. Will probably be fixed by adding real-time clocks later on.
* **Data protocol**: Need to actually send data! Not sure what format it should take. Maybe we can do JSON if we can afford the bandwidth.


# Next Time
- Make a demo!
- Heartbeat from root node sends time.
- Start with root node on. Turn on nearby nodes.
- Nodes start sending data they read from a sensor to root in their given frame.
- Turn one off, then back on. Should be able to rejoin and be good.
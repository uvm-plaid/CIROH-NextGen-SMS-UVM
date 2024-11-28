
# # Send JSON packet
# PACKET_PATH=sample_packet.json
# PAYLOAD=$(cat $PACKET_PATH)
# echo "Payload: $PAYLOAD"
# 
# curl --header "Content-Type: application/json" \
#   --header "Accept: application/json" \
#   --data @$PACKET_PATH \
#   https://jbourde2.w3.uvm.edu/ciroh/save_packet

# Send full file
RECORDING_PATH=sample_recording.wav
curl -X POST \
    -F "recording=@$RECORDING_PATH" \
    https://jbourde2.w3.uvm.edu/ciroh/save_file

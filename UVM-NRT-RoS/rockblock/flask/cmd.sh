
# Send packet as test
PAYLOAD=$(cat sample_packet.json)
echo "Payload: $PAYLOAD"

curl --header "Content-Type: application/json" \
  --header "Accept: application/json" \
  --data @sample_packet.json \
  https://jbourde2.w3.uvm.edu/ciroh/save_packet

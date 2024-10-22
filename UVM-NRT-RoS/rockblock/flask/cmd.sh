PAYLOAD='{"s":0,"t":30.0,"h":67.4,"p":1,"i":2}'
echo "Payload: $PAYLOAD"

curl --header "Content-Type: application/json" \
  --request POST \
  --data $PAYLOAD \
  https://jbourde2.w3.uvm.edu/ciroh/save

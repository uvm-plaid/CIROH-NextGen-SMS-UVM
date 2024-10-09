#include "shared.h"

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void basic_setup(uint8_t thisAddress, uint8_t toAddress) {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(100);

  Serial.println("Feather LoRa RX Test!");

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
  rf95.setThisAddress(thisAddress);
  rf95.setHeaderFrom(thisAddress);
  rf95.setHeaderTo(toAddress);
}

void blink() {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
}

struct TimePacket {
    uint32_t sec;
    uint32_t msec;
};

static struct Time {
    uint32_t sec;
    uint32_t msec;
    uint32_t localMsec;
} lastReceivedTime = {
    0,
    0,
    0,
};

enum PacketType {
    TIME,
    DATA,
};

static size_t memcpysz(char* dst, size_t dstsz, const char* src, size_t srcsz) {
    if (dstsz < srcsz) return 0;
    memcpy(dst, src, srcsz);
    return srcsz;
}

static size_t writeTime(TimePacket time, char* buf, size_t size) {
    return memcpysz(buf, size, (char*)&time, sizeof(time));
}

bool sendTime() {
    div_t now = div(millis(), 1000);
    TimePacket time = {
        now.quot,
        now.rem
    };
    static unsigned char sendBuf[256];
    size_t head = 0;
    sendBuf[head++] = PacketType::TIME;
    head += writeTime(time, (char*)sendBuf + head, sizeof(sendBuf) - head);
    bool success = rf95.send((uint8_t*)sendBuf, head);
    rf95.waitPacketSent();
    Serial.print("Sending time packet: ");
    for (unsigned int i = 0; i < head; i++) {
        int x = sendBuf[i];
        Serial.print(x, HEX);
        Serial.print(" ");
    }
    /*
    Serial.print(sendBuf[0], DEC);
    Serial.print(" ");
    uint32_t s;
    memcpy(&s, &sendBuf[1], sizeof(s));
    uint32_t ms;
    memcpy(&ms, &sendBuf[5], sizeof(ms));
    Serial.print(s, DEC);
    Serial.print(" ");
    Serial.print(ms, DEC);
    */
    Serial.println();
    return success;
}

size_t recvTime(const uint8_t* buf, size_t len) {
    TimePacket tp;
    size_t lenRead = memcpysz((char*)&tp, sizeof(tp), (char*)buf, len);
    if (lenRead != 0) {
        lastReceivedTime.sec = tp.sec;
        lastReceivedTime.msec = tp.msec;
        lastReceivedTime.localMsec = millis();
    }
}

int32_t currentNode() {
    div_t d = div(nowMs(), nodeWindowMs);
    uint32_t distanceFromNextNode = nodeWindowMs - d.rem;
    if (distanceFromNextNode < guardMs) return -1;

    int32_t node = (nowMs() / nodeWindowMs) % nodes;

    return node;
}

bool isCurrentNode(uint32_t node) {
    return currentNode() == node;
}

uint32_t nowMs() {
    return lastReceivedTime.sec * 1000 + lastReceivedTime.msec + millis() - lastReceivedTime.localMsec;
}
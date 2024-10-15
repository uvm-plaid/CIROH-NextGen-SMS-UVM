#include "tdm.h"
#include "loraDevice.h"
#include <Arduino.h>
#include <stdarg.h>

#pragma pack(push,1)
struct TimePacket {
    uint32_t sec;
    uint32_t msec;
};
#pragma pack(pop)

static struct Time {
    uint32_t sec;
    uint32_t msec;
    uint32_t localMsec;
} lastReceivedTime = {
    0,
    0,
    0,
};

const char* Vsprintf(const char* fmt, va_list args)
{
    static char strbuf[32] = {0};
    vsnprintf(strbuf, sizeof(strbuf), fmt, args);
    return strbuf;
}

const char* Sprintf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  const char* str = Vsprintf(fmt, args);
  va_end(args);
  return str;
}

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
    #ifdef ARDUINO_SAMD_MKRWAN1310
    return true;
    #else
    div_t now = div(millis(), 1000);
    TimePacket time = {
        now.quot,
        now.rem
    };
    static unsigned char sendBuf[16];
    size_t head = 0;
    sendBuf[head++] = PacketType::TIME;
    head += writeTime(time, (char*)sendBuf + head, sizeof(sendBuf) - head);

    // Dependency injection > global reference, but for now it's fine
    bool success = LoraDevice::getInstance()->send((uint8_t*)sendBuf, head);
    Serial.print("Sending time packet: ");
    for (unsigned int i = 0; i < head; i++) {
        int x = sendBuf[i];
        Serial.print(x, HEX);
        Serial.print(" ");
    }
    Serial.println();
    return success;
    #endif
}

size_t recvTime(const uint8_t* buf, size_t len) {
    TimePacket tp;
    size_t lenRead = memcpysz((char*)&tp, sizeof(tp), (char*)buf, len);
    if (lenRead != 0) {
        lastReceivedTime.sec = tp.sec;
        lastReceivedTime.msec = tp.msec;
        lastReceivedTime.localMsec = millis();
    }
    return lenRead;
}

int32_t currentNode() {
    div_t d = div((int)nowMs(), (int)nodeWindowMs);
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
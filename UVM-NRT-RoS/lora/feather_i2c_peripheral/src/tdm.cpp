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

struct Time {
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

size_t memcpysz(char* dst, size_t dstsz, const char* src, size_t srcsz) {
    if (dstsz < srcsz) return 0;
    memcpy(dst, src, srcsz);
    return srcsz;
}

size_t writeTime(TimePacket time, char* buf, size_t size) {
    return memcpysz(buf, size, (char*)&time, sizeof(time));
}

size_t sendTime(uint8_t** buffer) {
    #ifdef ARDUINO_SAMD_MKRWAN1310
    return true;
    #else
    static uint8_t sendBuf[16];
    size_t head = 0;
    sendBuf[head++] = PacketType::TIME;
    *(uint32_t*)&sendBuf[head] = millis();
    head += sizeof(uint32_t);

    // Dependency injection > global reference, but for now it's fine
    bool success = LoraDevice::getInstance()->send((uint8_t*)sendBuf, head);
    Serial.print("Sending time packet: ");
    for (unsigned int i = 0; i < head; i++) {
        int x = sendBuf[i];
        Serial.print(x, HEX);
        Serial.print(" ");
    }
    Serial.println();
    if (buffer != nullptr) {
        *buffer = sendBuf;
    }
    return head;
    #endif
}

template <typename T>
bool ReadPacket(T* t, const char* buf, size_t bufsize, size_t offset) {
    if (bufsize < offset + sizeof(T)) {
        return false;
    }

    return *(T*)&buf[offset];
}

bool ReadPacketId(uint8_t* id, const uint8_t* buf, size_t bufsize) {
    return ReadPacket(id, (const char*)buf, bufsize, 0);
}

bool ReadPacketMsec(uint32_t* msec, const uint8_t* buf, size_t bufsize) {
    return ReadPacket(msec, (const char*)buf, bufsize, 1);
}

int recvTime(uint8_t** buffer) {
    static uint8_t buf[16];
    const size_t len = sizeof(buf);
    if (buffer != nullptr) {
        *buffer = buf;
    }
    TimePacket tp;
    size_t lenRead = LoraDevice::getInstance()->recv(buf, len);
    if (lenRead <= 0) return 0;

    uint8_t packetType;
    if (!ReadPacketId(&packetType, buf, lenRead) || packetType != 0) {
        return -1;
    }

    if (!ReadPacketMsec(&lastReceivedTime.msec, buf, lenRead)) {
        return -1;
    }

    // Update time of last received packet.
    lastReceivedTime.localMsec = millis();

    return lenRead;
}

int32_t currentNode() {
    div_t d = div((int)nowMs(), (int)nodeWindowMs);
    uint32_t distanceFromNextNode = nodeWindowMs - d.rem;
    if (distanceFromNextNode < guardMs) return -1;

    int32_t node = d.quot % nodes;

    return node;
}

bool isCurrentNode(uint32_t node) {
    return currentNode() == node;
}

bool isConnected() {
    return !(lastReceivedTime.localMsec == 0 && lastReceivedTime.msec == 0 && lastReceivedTime.sec == 0);
}

uint32_t lastReceivedTimeMsec()
{
    return lastReceivedTime.msec;
}

uint32_t lastReceivedTimeSec()
{
    return lastReceivedTime.sec;
}

uint32_t lastReceivedTimeLocalMsec()
{
    return lastReceivedTime.msec;
}

uint32_t nowMs() {
    return lastReceivedTime.sec * 1000 + lastReceivedTime.msec + millis() - lastReceivedTime.localMsec;
}
#include <stdint.h>
#include <stdarg.h>

const uint32_t nodes = 4;
const uint32_t nodeWindowMs = 200;
const uint32_t frameWindow = nodeWindowMs * 4;
const uint32_t guardMs = 10;

bool sendTime();
bool recvPacket();

// Returns the current node, or -1 if we're in a guard period.
int32_t currentNode();

bool isCurrentNode(uint32_t node);
uint32_t nowMs();

const char* Vsprintf(const char* fmt, va_list args);
const char* Sprintf(const char* fmt, ...);
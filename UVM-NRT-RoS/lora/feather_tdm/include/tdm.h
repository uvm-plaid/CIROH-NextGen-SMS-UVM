#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

const uint32_t nodes = 4;
const uint32_t nodeWindowMs = 1500;
const uint32_t frameWindow = nodeWindowMs * 4;
const uint32_t guardMs = 500;

size_t sendTime(uint8_t** buffer = nullptr);
int recvTime(uint8_t** buffer = nullptr);
bool isConnected();

// Returns the current node, or -1 if we're in a guard period.
int32_t currentNode();

constexpr int32_t TIME_NODE = 0;

bool isCurrentNode(uint32_t node);
uint32_t nowMs();

const char* Vsprintf(const char* fmt, va_list args);
const char* Sprintf(const char* fmt, ...);
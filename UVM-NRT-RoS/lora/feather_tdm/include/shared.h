#include <RH_RF95.h>

const uint32_t nodes = 4;
const uint32_t nodeWindowMs = 200;
const uint32_t frameWindow = nodeWindowMs * 4;
const uint32_t guardMs = 10;

struct vtable_t {
  void (*setup)(RHGenericDriver& rf95);
  void (*loop)(RHGenericDriver& rf95);
};

extern vtable_t tx_vtable;
extern vtable_t rx_vtable;

extern RH_RF95 rf95;

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define LED 13 // Built-in LED
#define RF95_FREQ 915.0

void basic_setup(uint8_t thisAddress, uint8_t toAddress);
void blink();

bool sendTime();
bool recvPacket();

// Returns the current node, or -1 if we're in a guard period.
int32_t currentNode();
bool isCurrentNode(uint32_t node);
uint32_t nowMs();
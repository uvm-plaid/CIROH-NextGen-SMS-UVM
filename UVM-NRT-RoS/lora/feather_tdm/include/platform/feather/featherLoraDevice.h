#include "loraDevice.h"

class LilyGoLoraDevice : public LoraDevice
{
public:
    void setup() override;
    int32_t recv(uint8_t* buffer, uint8_t length) override;
    int32_t send(uint8_t* buffer, uint8_t length) override;
    bool isAvailable() override;
    int16_t lastRssi() override;
    void setCrcEnabled(bool enabled) override;
};

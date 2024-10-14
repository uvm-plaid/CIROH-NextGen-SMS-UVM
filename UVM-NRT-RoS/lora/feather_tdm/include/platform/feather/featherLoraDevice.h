#include "loraDevice.h"

class FeatherLoraDevice : public LoraDevice
{
public:
    void setup() override;
    int32_t recv(uint8_t* buffer, uint8_t length) override;
    int32_t send(const uint8_t* buffer, uint8_t length) override;
    bool isAvailable() override;
    int16_t lastRssi() override;
    float lastSnr() override;

    void setCrcEnabled(bool enabled) override;
};

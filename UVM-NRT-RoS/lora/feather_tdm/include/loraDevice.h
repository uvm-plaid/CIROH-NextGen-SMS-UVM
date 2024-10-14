#pragma once

#include <stdint.h>

class LoraDevice
{
public:
    virtual void setup() = 0;
    virtual int32_t recv(uint8_t* buffer, uint8_t length) = 0;
    virtual int32_t send(const uint8_t* buffer, uint8_t length) = 0;
    virtual bool isAvailable() = 0;
    virtual int16_t lastRssi() = 0;
    virtual float lastSnr() = 0;

    virtual void setCrcEnabled(bool enabled) = 0;

    static LoraDevice* getInstance() { return Instance; }

private:
    static LoraDevice* Instance;
};
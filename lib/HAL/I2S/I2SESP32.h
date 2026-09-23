#pragma once

#include "I2S.h"

namespace Sophia::Platform::ArduinoESP32 {

class I2SESP32 final : public Sophia::HAL::I2S {

public:

    I2SESP32() = default;
    ~I2SESP32() override = default;

    Sophia::HAL::I2SError begin(
        const Sophia::HAL::I2SConfig& config
    ) override;

    void end() override;

    size_t read(
        int16_t* buffer,
        size_t samples
    ) override;

private:

    bool initialized = false;
};

}
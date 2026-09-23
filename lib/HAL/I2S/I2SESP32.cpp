#include "I2SESP32.h"

#include <ESP_I2S.h>

namespace {

I2SClass i2s;

}

namespace Sophia::Platform::ArduinoESP32 {

Sophia::HAL::I2SError I2SESP32::begin(
    const Sophia::HAL::I2SConfig& config
) {

    i2s.setPins(
        config.pins.bck,
        config.pins.ws,
        -1,
        config.pins.data
    );

    const bool started = i2s.begin(
        I2S_MODE_STD,
        config.sampleRate,
        I2S_DATA_BIT_WIDTH_16BIT,
        I2S_SLOT_MODE_MONO,
        I2S_STD_SLOT_LEFT
    );

    if (!started) {

        initialized = false;

        return Sophia::HAL::I2SError::DriverStartFailed;
    }

    initialized = true;

    return Sophia::HAL::I2SError::None;
}

void I2SESP32::end() {

    if (!initialized) {
        return;
    }

    i2s.end();

    initialized = false;
}

size_t I2SESP32::read(
    int16_t* buffer,
    size_t samples
) {

    if (!initialized || buffer == nullptr || samples == 0) {
        return 0;
    }

    const size_t bytesToRead =
        samples * sizeof(int16_t);

    const size_t bytesRead =
        i2s.readBytes(
            reinterpret_cast<char*>(buffer),
            bytesToRead
        );

    return bytesRead / sizeof(int16_t);
}

}
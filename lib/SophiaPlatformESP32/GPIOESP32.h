#pragma once

#include <GPIO.h>

namespace Sophia::Platform::ArduinoESP32 {

class GPIOESP32 final : public Sophia::HAL::GPIO {

public:

    void output(uint8_t pin) override;
    void input(uint8_t pin) override;
    void write(uint8_t pin, bool value) override;
    bool read(uint8_t pin) override;
    void toggle(uint8_t pin) override;
};

}
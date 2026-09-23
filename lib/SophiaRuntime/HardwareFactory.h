#pragma once

#include <GPIO.h>
#include <I2S/I2S.h>

namespace Sophia::Runtime {

    class HardwareFactory final {

    public:

        static Sophia::HAL::GPIO& gpio();

        static Sophia::HAL::I2S& i2s();
    };
}
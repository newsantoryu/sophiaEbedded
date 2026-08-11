#pragma once

#include <GPIO.h>
#include <GPIOESP32.h>

namespace Sophia::Runtime {

    class HardwareFactory final {

        public:

        static Sophia::HAL::GPIO& gpio();
    };
}
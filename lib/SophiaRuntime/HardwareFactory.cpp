#include "HardwareFactory.h"

namespace {

    Sophia::Platform::ArduinoESP32::GPIOESP32 gpioInstance;

}

namespace Sophia::Runtime {

    Sophia::HAL::GPIO& HardwareFactory::gpio() {
        return gpioInstance;
    }

}
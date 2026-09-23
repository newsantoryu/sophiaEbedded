#include "HardwareFactory.h"

#include <GPIOESP32.h>
#include <I2S/I2SESP32.h>

namespace {

Sophia::Platform::ArduinoESP32::GPIOESP32 gpioInstance;
Sophia::Platform::ArduinoESP32::I2SESP32 i2sInstance;

}

namespace Sophia::Runtime {

Sophia::HAL::GPIO& HardwareFactory::gpio() {
    return gpioInstance;
}

Sophia::HAL::I2S& HardwareFactory::i2s() {
    return i2sInstance;
}

}
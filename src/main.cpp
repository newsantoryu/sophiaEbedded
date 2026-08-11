#include <Arduino.h>

#include "GPIOConfig.h"
#include "HardwareFactory.h"
#include "I2S/I2SESP32.h"
#include "UART.h"

namespace {

Sophia::HAL::GPIO& gpio =
    Sophia::Runtime::HardwareFactory::gpio();

Sophia::HAL::I2SESP32 i2s;

}

void setup() {
    Serial.begin(115200);
    delay(2000);

    Sophia::HAL::UART::println("Sophia Embedded Boot");

    gpio.output(
        Sophia::Config::GPIOConfig::StatusLED
    );

    Sophia::HAL::I2SConfig config;
    config.sampleRate = 16000;

    const auto result = i2s.begin(config);

    if (result == Sophia::HAL::I2SError::None) {
        Sophia::HAL::UART::println(
            "I2S Initialized Successfully"
        );
    } else {
        Sophia::HAL::UART::println(
            "Failed to initialize I2S"
        );
    }
}

void loop() {
    gpio.toggle(
        Sophia::Config::GPIOConfig::StatusLED
    );

    Sophia::HAL::UART::println(
        "Sophia Capture Audio Heartbeat"
    );

    delay(1000);
}
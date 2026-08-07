#include <Arduino.h>

#include "GPIOESP32.h"

namespace Sophia::Platform::ArduinoESP32 {

void GPIOESP32::output(uint8_t pin) {
    pinMode(pin, OUTPUT);
}

void GPIOESP32::input(uint8_t pin) {
    pinMode(pin, INPUT);
}

void GPIOESP32::write(uint8_t pin, bool value) {
    digitalWrite(pin, value ? HIGH : LOW);
}

bool GPIOESP32::read(uint8_t pin) {
    return digitalRead(pin) == HIGH;
}

void GPIOESP32::toggle(uint8_t pin) {
    write(pin, !read(pin));
}

}
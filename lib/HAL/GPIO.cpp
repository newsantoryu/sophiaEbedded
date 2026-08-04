#include "GPIO.h"
#include <Arduino.h>

namespace Sophia::HAL {

    void GPIO::output(uint8_t pin) {

        pinMode(pin, OUTPUT);

    }

    void GPIO::input(uint8_t pin) {

        pinMode(pin, INPUT);

    }

    void GPIO::high(uint8_t pin) {

        digitalWrite(pin, HIGH);

    }

    void GPIO::low(uint8_t pin) {

        digitalWrite(pin, LOW);

    }

    void GPIO::toggle(uint8_t pin) {

        digitalWrite(pin, !digitalRead(pin));

    }
}

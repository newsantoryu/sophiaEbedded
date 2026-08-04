#include "UART.h"

namespace Sophia::HAL {

    void UART::begin(uint32_t baudrate) {
        Serial.begin(baudrate);
    }

    void UART::write(const char* message) {
        Serial.write(message);
    }

    void UART::write(const uint8_t* data, size_t size) {
        Serial.write(data, size);
    }

    void UART::println(const char* message) {
        Serial.println(message);
    }
}
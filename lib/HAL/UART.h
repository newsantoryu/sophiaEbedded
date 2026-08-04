#pragma once

#include<Arduino.h>

namespace Sophia::HAL {

    class UART {
    public:
        static void begin(uint32_t baudrate);
        static void write(const char* message);
        static void write(const uint8_t* data, size_t size); 
        static void println(const char* message);
    };

}
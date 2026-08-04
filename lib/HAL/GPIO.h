#pragma once 

#include <stdint.h>

namespace Sophia::HAL {
    
    class GPIO {
        
        public:

            static void output(uint8_t pin);

            static void input(uint8_t pin);

            static void high(uint8_t pin);

            static void low(uint8_t pin);

            static void toggle(uint8_t pin);
    };
}

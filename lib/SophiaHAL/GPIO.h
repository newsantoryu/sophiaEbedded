#pragma once 

#include <stdint.h>

namespace Sophia::HAL {
    
    class GPIO {
        
        public:

        virtual ~GPIO() = default;

        virtual void output(uint8_t pin) = 0;

        virtual void input(uint8_t pin) = 0;

        virtual void write(uint8_t pin, bool value) = 0;

        virtual bool read(uint8_t pin) = 0;

        virtual void toggle(uint8_t pin) = 0;
        
    };
}

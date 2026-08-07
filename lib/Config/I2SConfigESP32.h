#pragma once
#include <cstdint>

namespace Sophia:: Config {
    struct I2SConfigESP32 {

        static constexpr uint8_t BCK  = 26;

        static constexpr uint8_t WS  = 25;

        static constexpr uint8_t DATA  = 22;
        
    };
}
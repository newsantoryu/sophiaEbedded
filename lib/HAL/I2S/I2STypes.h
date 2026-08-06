#pragma once

#include <stdint.h>

namespace Sophia::HAL {

    enum class I2SMode : uint8_t {
        Receiver,
        Transmitter
    };


    enum class I2SChannels : uint8_t {
        Mono = 1,
        Stereo = 2
    };

    enum class I2SBitDepth : uint8_t {
        BitDepth16 = 16,
        BitDepth24 = 24,
        BitDepth32 = 32
    };

}
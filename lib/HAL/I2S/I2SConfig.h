#pragma once

#include <stdint.h>

#include "I2STypes.h"

namespace Sophia::HAL {

    //GPIOs utilizados pelo periféricos I2S.
    struct I2SPins {
        
        uint8_t bck;
        uint8_t ws;
        uint8_t data;

    };


    // Configuração independente da plataforma para um dispositivo I2S.
    struct I2SConfig {

        uint32_t sampleRate = 16000;

        I2SBitDepth bitDepth = I2SBitDepth::BitDepth32;

        I2SChannels channels = I2SChannels::Mono;

        I2SMode mode = I2SMode::Receiver;

        size_t dmaBufferLength = 512;

        I2SPins pins;

    };

























}
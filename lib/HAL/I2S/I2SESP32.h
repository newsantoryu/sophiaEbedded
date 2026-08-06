#pragma once

#include "I2S.h"

namespace Sophia::HAL {

    class I2SESP32 final : public I2S {
        public:

            I2SESP32() = default;
            ~I2SESP32() override = default;

            I2SError begin(const I2SConfig& config) override;

            void end() override;

            size_t read(int32_t* buffer, size_t samples) override;

        private:

            bool initialized = false;
            
    };
}
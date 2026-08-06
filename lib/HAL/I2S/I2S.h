#include <stddef.h>
#include <stdint.h>

#include "I2SConfig.h"
#include "I2SError.h"

namespace Sophia::HAL {

    class I2S {

        public:
            virtual ~I2S() = default;

            virtual I2SError begin(const I2SConfig& config) = 0;

            virtual void end() = 0;

            virtual size_t read(int32_t* buffer, size_t samples) = 0;
    };
}
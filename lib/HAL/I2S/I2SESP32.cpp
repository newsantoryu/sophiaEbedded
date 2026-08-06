#include "I2SESP32.h"

using namespace Sophia::HAL;

I2SError I2SESP32::begin(const I2SConfig& config) {
    initialized = true;
    return I2SError::None; // Assuming I2SError::None indicates success
}

void I2SESP32::end() {
    initialized = false;
}

size_t I2SESP32::read(
    int32_t*, 
    size_t
) {
    return 0;
}


#pragma once

namespace Sophia::HAL
{
    enum class I2SError {

        None,
        InvalidConfiguration,
        DiverInstallFailed,
        DriverStartFailed,
        ReadFailed,
        WriteFailed,
        Timeout,
        NotInitialized

    };
} 
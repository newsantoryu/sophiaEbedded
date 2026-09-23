#include "VoiceActivityDetector.h"

namespace Sophia::Audio {

VoiceActivityDetector::VoiceActivityDetector(
    const VoiceActivityConfig& config
)
    : config(config) {
}

VoiceActivityState VoiceActivityDetector::update(
    double energy
) {

    // ========================================================
    // SILENCE
    // ========================================================

    if (
        currentState ==
        VoiceActivityState::Silence
    ) {

        stopCounter = 0;

        if (
            energy >=
            config.startThreshold
        ) {

            ++startCounter;

            if (
                startCounter >=
                config.startConfirmationBlocks
            ) {

                currentState =
                    VoiceActivityState::Voice;

                startCounter = 0;
            }

        } else {

            startCounter = 0;
        }
    }

    // ========================================================
    // VOICE
    // ========================================================

    else {

        startCounter = 0;

        if (
            energy <=
            config.stopThreshold
        ) {

            ++stopCounter;

            if (
                stopCounter >=
                config.stopConfirmationBlocks
            ) {

                currentState =
                    VoiceActivityState::Silence;

                stopCounter = 0;
            }

        } else {

            stopCounter = 0;
        }
    }

    return currentState;
}

// ============================================================
// Estado atual
// ============================================================

VoiceActivityState VoiceActivityDetector::state() const {

    return currentState;
}

// ============================================================
// Reset
// ============================================================

void VoiceActivityDetector::reset() {

    currentState =
        VoiceActivityState::Silence;

    startCounter = 0;
    stopCounter = 0;
}

}

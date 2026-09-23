#pragma once

#include <stddef.h>

namespace Sophia::Audio {

enum class VoiceActivityState {
    Silence,
    Voice
};

struct VoiceActivityConfig {

    // Energia necessária para iniciar VOICE.
    double startThreshold = 35.0;

    // Energia abaixo da qual podemos retornar para SILENCE.
    double stopThreshold = 25.0;

    // Quantos blocos consecutivos acima do START
    // são necessários para entrar em VOICE.
    size_t startConfirmationBlocks = 3;

    // Quantos blocos consecutivos abaixo do STOP
    // são necessários para retornar para SILENCE.
    size_t stopConfirmationBlocks = 5;
};

class VoiceActivityDetector {

public:

    explicit VoiceActivityDetector(
        const VoiceActivityConfig& config
    );

    VoiceActivityState update(
        double energy
    );

    VoiceActivityState state() const;

    void reset();

private:

    VoiceActivityConfig config;

    VoiceActivityState currentState =
        VoiceActivityState::Silence;

    size_t startCounter = 0;
    size_t stopCounter = 0;
};

}
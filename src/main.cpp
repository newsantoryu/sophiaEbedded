#include <Arduino.h>
#include <math.h>

#include "GPIOConfig.h"
#include "HardwareFactory.h"
#include "UART.h"
#include "I2S/I2S.h"
#include "Audio/VoiceActivityDetector.h"

namespace {

Sophia::HAL::GPIO& gpio =
    Sophia::Runtime::HardwareFactory::gpio();

Sophia::HAL::I2S& i2s =
    Sophia::Runtime::HardwareFactory::i2s();

constexpr size_t AudioBufferSize = 256;
constexpr size_t MeasurementBlocks = 50;

// ============================================================
// VAD
// ============================================================

Sophia::Audio::VoiceActivityConfig vadConfig;

Sophia::Audio::VoiceActivityDetector vad(
    vadConfig
);

// ============================================================
// Energy Envelope
// ============================================================

// Cada bloco:
// 256 samples / 16000 Hz = 16 ms

constexpr size_t SmoothingWindow = 5;

// 5 blocos × 16 ms = 80 ms
constexpr size_t MaxBarWidth = 50;

// Somente escala visual.
// NÃO é threshold de VAD.
constexpr double DisplayScale = 200.0;

int16_t audioBuffer[AudioBufferSize];

// Buffer circular utilizado pela média móvel.
double envelopeWindow[SmoothingWindow] = {};

size_t envelopeWindowIndex = 0;
size_t envelopeWindowCount = 0;

double envelopeWindowSum = 0.0;

}

// ============================================================
// Calcula RMS AC de um bloco
// ============================================================

double calculateRmsAc(
    const int16_t* buffer,
    size_t samples
) {

    if (
        buffer == nullptr ||
        samples == 0
    ) {
        return 0.0;
    }

    // --------------------------------------------------------
    // 1. Calcula DC / média do bloco
    // --------------------------------------------------------

    int64_t sum = 0;

    for (
        size_t i = 0;
        i < samples;
        ++i
    ) {

        sum += buffer[i];
    }

    const double mean =
        static_cast<double>(sum) /
        samples;

    // --------------------------------------------------------
    // 2. Calcula energia AC
    // --------------------------------------------------------

    double sumAcOfSquares = 0.0;

    for (
        size_t i = 0;
        i < samples;
        ++i
    ) {

        const double sample =
            static_cast<double>(
                buffer[i]
            );

        const double acSample =
            sample - mean;

        sumAcOfSquares +=
            acSample * acSample;
    }

    const double meanAcSquare =
        sumAcOfSquares /
        samples;

    return sqrt(meanAcSquare);
}

// ============================================================
// Atualiza média móvel do envelope
// ============================================================

double updateSmoothedEnvelope(
    double rmsAc
) {

    // Se a janela já estiver cheia,
    // remove o valor mais antigo.
    if (
        envelopeWindowCount ==
        SmoothingWindow
    ) {

        envelopeWindowSum -=
            envelopeWindow[
                envelopeWindowIndex
            ];

    } else {

        ++envelopeWindowCount;
    }

    // Adiciona novo valor.
    envelopeWindow[
        envelopeWindowIndex
    ] = rmsAc;

    envelopeWindowSum += rmsAc;

    // Avança índice circular.
    envelopeWindowIndex =
        (
            envelopeWindowIndex + 1
        ) %
        SmoothingWindow;

    return
        envelopeWindowSum /
        envelopeWindowCount;
}

// ============================================================
// Desenha barra do envelope
// ============================================================

String createEnergyBar(
    double value
) {

    double normalized =
        value / DisplayScale;

    if (normalized > 1.0) {
        normalized = 1.0;
    }

    if (normalized < 0.0) {
        normalized = 0.0;
    }

    const size_t barLength =
        static_cast<size_t>(
            normalized * MaxBarWidth
        );

    String bar;

    for (
        size_t i = 0;
        i < barLength;
        ++i
    ) {

        bar += '#';
    }

    return bar;
}

// ============================================================
// SETUP
// ============================================================

void setup() {

    Serial.begin(115200);

    delay(2000);

    Sophia::HAL::UART::println(
        "Sophia Embedded Boot"
    );

    gpio.output(
        Sophia::Config::GPIOConfig::StatusLED
    );

    gpio.write(
        Sophia::Config::GPIOConfig::StatusLED,
        true
    );

    Sophia::HAL::UART::println(
        "Status LED ON"
    );

    Sophia::HAL::I2SConfig config;

    config.sampleRate = 16000;

    config.bitDepth =
        Sophia::HAL::I2SBitDepth::BitDepth16;

    config.channels =
        Sophia::HAL::I2SChannels::Mono;

    config.mode =
        Sophia::HAL::I2SMode::Receiver;

    config.pins.bck = 26;
    config.pins.ws = 25;
    config.pins.data = 22;

    const auto result =
        i2s.begin(config);

    if (
        result ==
        Sophia::HAL::I2SError::None
    ) {

        Sophia::HAL::UART::println(
            "I2S Initialized Successfully"
        );

    } else {

        Sophia::HAL::UART::println(
            "Failed to initialize I2S"
        );

        gpio.write(
            Sophia::Config::GPIOConfig::StatusLED,
            false
        );
    }
}

// ============================================================
// LOOP
// ============================================================

void loop() {

    // --------------------------------------------------------
    // Descarta primeiro bloco.
    // --------------------------------------------------------

    const size_t firstRead =
        i2s.read(
            audioBuffer,
            AudioBufferSize
        );

    if (firstRead == 0) {

        Sophia::HAL::UART::println(
            "I2S: No samples"
        );

        delay(100);

        return;
    }

    // --------------------------------------------------------
    // Reinicia estado da média móvel
    // para cada nova janela de medição.
    // --------------------------------------------------------

    for (
        size_t i = 0;
        i < SmoothingWindow;
        ++i
    ) {

        envelopeWindow[i] = 0.0;
    }

    envelopeWindowIndex = 0;
    envelopeWindowCount = 0;
    envelopeWindowSum = 0.0;

    // --------------------------------------------------------
    // Reinicia estado do VAD.
    // Cada medição começa em SILENCE.
    // --------------------------------------------------------

    vad.reset();

    Sophia::HAL::UART::println("");

    Sophia::HAL::UART::println(
        "====== ENERGY ENVELOPE + VAD ======"
    );

    Sophia::HAL::UART::println(
        "Window: 5 blocks / 80 ms"
    );

    Sophia::HAL::UART::println(
        "VAD Start: 35.0"
    );

    Sophia::HAL::UART::println(
        "VAD Stop: 25.0"
    );

    Sophia::HAL::UART::println(
        "VAD Start Confirmation: 3 blocks"
    );

    Sophia::HAL::UART::println(
        "VAD Stop Confirmation: 5 blocks"
    );

    double rmsAcSum = 0.0;
    double smoothedSum = 0.0;

    double rmsAcMinimum = 0.0;
    double rmsAcMaximum = 0.0;

    double smoothedMinimum = 0.0;
    double smoothedMaximum = 0.0;

    size_t validBlocks = 0;

    // --------------------------------------------------------
    // Processa 50 blocos
    // --------------------------------------------------------

    for (
        size_t block = 0;
        block < MeasurementBlocks;
        ++block
    ) {

        const size_t samplesRead =
            i2s.read(
                audioBuffer,
                AudioBufferSize
            );

        if (samplesRead == 0) {

            Sophia::HAL::UART::println(
                "I2S: No samples"
            );

            continue;
        }

        // ----------------------------------------------------
        // RMS AC
        // ----------------------------------------------------

        const double rmsAc =
            calculateRmsAc(
                audioBuffer,
                samplesRead
            );

        // ----------------------------------------------------
        // Envelope suavizado
        // ----------------------------------------------------

        const double smoothedEnvelope =
            updateSmoothedEnvelope(
                rmsAc
            );

        // ----------------------------------------------------
        // VAD
        // ----------------------------------------------------

        const auto vadState =
            vad.update(
                smoothedEnvelope
            );

        const char* vadText =
            vadState ==
                Sophia::Audio::VoiceActivityState::Voice
                ? "VOICE"
                : "SILENCE";

        // ----------------------------------------------------
        // Estatísticas
        // ----------------------------------------------------

        rmsAcSum += rmsAc;
        smoothedSum += smoothedEnvelope;

        if (validBlocks == 0) {

            rmsAcMinimum = rmsAc;
            rmsAcMaximum = rmsAc;

            smoothedMinimum =
                smoothedEnvelope;

            smoothedMaximum =
                smoothedEnvelope;

        } else {

            if (rmsAc < rmsAcMinimum) {
                rmsAcMinimum = rmsAc;
            }

            if (rmsAc > rmsAcMaximum) {
                rmsAcMaximum = rmsAc;
            }

            if (
                smoothedEnvelope <
                smoothedMinimum
            ) {

                smoothedMinimum =
                    smoothedEnvelope;
            }

            if (
                smoothedEnvelope >
                smoothedMaximum
            ) {

                smoothedMaximum =
                    smoothedEnvelope;
            }
        }

        ++validBlocks;

        // ----------------------------------------------------
        // Visualização
        // ----------------------------------------------------

        const String rawBar =
            createEnergyBar(
                rmsAc
            );

        const String smoothBar =
            createEnergyBar(
                smoothedEnvelope
            );

        Sophia::HAL::UART::println(
            (
                String("Block ") +
                String(block + 1) +
                " | Raw: " +
                String(rmsAc, 2) +
                " | " +
                rawBar +
                " | Smooth: " +
                String(smoothedEnvelope, 2) +
                " | " +
                smoothBar +
                " | VAD: " +
                vadText
            ).c_str()
        );
    }

    // --------------------------------------------------------
    // Métricas
    // --------------------------------------------------------

    if (validBlocks > 0) {

        const double rmsAcAverage =
            rmsAcSum /
            validBlocks;

        const double smoothedAverage =
            smoothedSum /
            validBlocks;

        const auto finalVadState =
            vad.state();

        const char* finalVadText =
            finalVadState ==
                Sophia::Audio::VoiceActivityState::Voice
                ? "VOICE"
                : "SILENCE";

        Sophia::HAL::UART::println("");

        Sophia::HAL::UART::println(
            "------------ METRICS ------------"
        );

        Sophia::HAL::UART::println(
            (
                String("Blocks: ") +
                validBlocks
            ).c_str()
        );

        Sophia::HAL::UART::println(
            (
                String("Raw Average: ") +
                String(rmsAcAverage, 2)
            ).c_str()
        );

        Sophia::HAL::UART::println(
            (
                String("Raw Min: ") +
                String(rmsAcMinimum, 2)
            ).c_str()
        );

        Sophia::HAL::UART::println(
            (
                String("Raw Max: ") +
                String(rmsAcMaximum, 2)
            ).c_str()
        );

        Sophia::HAL::UART::println(
            (
                String("Smooth Average: ") +
                String(smoothedAverage, 2)
            ).c_str()
        );

        Sophia::HAL::UART::println(
            (
                String("Smooth Min: ") +
                String(smoothedMinimum, 2)
            ).c_str()
        );

        Sophia::HAL::UART::println(
            (
                String("Smooth Max: ") +
                String(smoothedMaximum, 2)
            ).c_str()
        );

        Sophia::HAL::UART::println(
            (
                String("Final VAD State: ") +
                finalVadText
            ).c_str()
        );

        Sophia::HAL::UART::println(
            "---------------------------------"
        );
    }

    Sophia::HAL::UART::println(
        "================================="
    );

    delay(1000);
}

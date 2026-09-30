#pragma once
#include <Arduino.h>

namespace cfg
{
    // -------------------------------------------------
    // Motor driver pins
    // -------------------------------------------------
    constexpr uint8_t MOTOR_DIR_PIN = 3;
    constexpr uint8_t MOTOR_PWM_PIN = 6;

    // -------------------------------------------------
    // Built-in encoder pins
    // -------------------------------------------------
    constexpr uint8_t BUILTIN_ENC_A_PIN = 7;
    constexpr uint8_t BUILTIN_ENC_B_PIN = 8;

    // -------------------------------------------------
    // Serial
    // -------------------------------------------------
    constexpr unsigned long SERIAL_BAUD = 250000;

    // -------------------------------------------------
    // Timing
    // -------------------------------------------------
    constexpr unsigned long RUN_TIME_MS = 15500;
    constexpr unsigned long PI_PERIOD_MS = 13;
    constexpr unsigned long SPEED_SAMPLE_MS = 100;
    constexpr unsigned long DISPLAY_PERIOD_MS = 5000;

    // -------------------------------------------------
    // PI controller
    // Original BaseCodeOne values
    // -------------------------------------------------
    constexpr float KP = 0.4f;
    constexpr float KI = 0.01f;
    constexpr float BASE_PWM = 50.0f;

    // -------------------------------------------------
    // Built-in encoder
    // Original BaseCodeOne conversion
    // 2 × 114 = 228 counts/revolution
    // -------------------------------------------------
    constexpr float BUILTIN_COUNTS_PER_REV = 228.0f;
}
#pragma once

#include <Arduino.h>

namespace cfg
{
    constexpr uint32_t SERIAL_BAUD = 250000;

    constexpr uint8_t MOTOR_DIR_PIN = 3;
    constexpr uint8_t MOTOR_PWM_PIN = 6;

    constexpr uint8_t ENCODER_A_PIN = 7;
    constexpr uint8_t ENCODER_B_PIN = 8;

    constexpr uint8_t IR_1 = A4;
    constexpr uint8_t IR_2 = A3;
    constexpr uint8_t IR_3 = A2;
    constexpr uint8_t IR_4 = A1;
    constexpr uint8_t IR_5 = A0;
    constexpr uint8_t IR_6 = A5;

    constexpr float ENCODER_COUNTS_PER_REV = 228.0f;
    constexpr float IR_COUNTS_PER_REV = 64.0f;

    constexpr uint32_t POSITION_TIME_MS = 4000;

    constexpr int MAX_REPETITIONS = 10;

    constexpr float KI = 0.02f;
}
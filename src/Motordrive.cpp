#include <Arduino.h>

#include "MotorDriver.h"
#include "Config.h"

void MotorDriver::begin()
{
    pinMode(cfg::MOTOR_DIR_PIN, OUTPUT);
    pinMode(cfg::MOTOR_PWM_PIN, OUTPUT);

    analogWrite(cfg::MOTOR_PWM_PIN, 0);
}

void MotorDriver::setDirectionFromRPM(int commandedRPM)
{
    if (commandedRPM < 0)
    {
        // Same behaviour as supplied BaseCodeOne
        analogWrite(cfg::MOTOR_DIR_PIN, 255);
    }
    else
    {
        analogWrite(cfg::MOTOR_DIR_PIN, 0);
    }
}

void MotorDriver::setPWM(float pwm)
{
    // Intentionally keeping original BaseCodeOne behaviour.
    // Do not add constrain() yet because that would change
    // the supplied controller behaviour.
    analogWrite(cfg::MOTOR_PWM_PIN, pwm);
}

void MotorDriver::stop()
{
    analogWrite(cfg::MOTOR_PWM_PIN, 0);
}
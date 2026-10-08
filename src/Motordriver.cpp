#include <Arduino.h>

#include "MotorDriver.h"
#include "config.h"


void MotorDriver::setDirectionFromDegree(float degree)
{
    if (degree < 0)
    {
        analogWrite(cfg::MOTOR_DIR_PIN, 255);
    }
}


void MotorDriver::setPWM(float pwm)
{
    analogWrite(cfg::MOTOR_PWM_PIN, pwm);
}


void MotorDriver::stop()
{
    analogWrite(cfg::MOTOR_PWM_PIN, 0);
}
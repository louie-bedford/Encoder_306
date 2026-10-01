#include <Arduino.h>

#include "Sensor.h"
#include "config.h"


void Sensor::update()
{
    values_[0] = (analogRead(cfg::IR_1) > 1) ? 1.0f : 0.0f;
    values_[1] = (analogRead(cfg::IR_2) > 1) ? 1.0f : 0.0f;
    values_[2] = (analogRead(cfg::IR_3) > 1) ? 1.0f : 0.0f;
    values_[3] = (analogRead(cfg::IR_4) > 1) ? 1.0f : 0.0f;
    values_[4] = (analogRead(cfg::IR_5) > 1) ? 1.0f : 0.0f;
    values_[5] = (analogRead(cfg::IR_6) > 1) ? 1.0f : 0.0f;
}


const float* Sensor::getValue() const
{
    return values_;
}


void Sensor::reset()
{
    for (int i = 0; i < 6; i++)
    {
        values_[i] = 0;
    }
}
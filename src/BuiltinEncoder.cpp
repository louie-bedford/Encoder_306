#include <Arduino.h>

#include "BuiltinEncoder.h"
#include "config.h"


void BuiltinEncoder::update()
{
    channel1_ = digitalRead(cfg::ENCODER_A_PIN);
    channel2_ = digitalRead(cfg::ENCODER_B_PIN);


    if (channel1_ != channel2_ && state_ == 0)
    {
        counts_ = counts_ + 1;
        totalcount_ +=1;
        state_ = 1;
    }


    if (channel1_ == channel2_ && state_ == 1)
    {
        counts_ = counts_ + 1;
        totalcount_+=1;

        state_ = 0;
    }
}


float BuiltinEncoder::getCounts() const
{
    return counts_;
}

float BuiltinEncoder::gettotal() const
{
    return totalcount_;
}


float BuiltinEncoder::getAngleDegrees() const
{
    return counts_ * 360.0f / cfg::ENCODER_COUNTS_PER_REV;
}


void BuiltinEncoder::reset()
{
    counts_ = 0;
}

float BuiltinEncoder::getRPM(float elapsed) const
{
    if (elapsed <= 0.0f)
        return 0.0f;
    float rpm = counts_/(cfg::ENCODER_COUNTS_PER_REV * elapsed) * 60000.0;
    return rpm;
} 
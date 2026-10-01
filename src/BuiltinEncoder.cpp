#include <Arduino.h>

#include "BuiltinEncoder.h"
#include "config.h"

void BuiltinEncoder::begin()
{
    pinMode(cfg::BUILTIN_ENC_A_PIN, INPUT);
    pinMode(cfg::BUILTIN_ENC_B_PIN, INPUT);

    displayCount_ = 0.0f;
    controllerCount_ = 0.0f;

    repeatState_ = 0;
    previousChannelB_ = 0;

    directionCounter_ = 0;
    direction_ = 0;
}


void BuiltinEncoder::updateCounts()
{
    channelA_ = digitalRead(cfg::BUILTIN_ENC_A_PIN);
    channelB_ = digitalRead(cfg::BUILTIN_ENC_B_PIN);

    // Original:
    //
    // if (s1 != s2 && r == 0)
    //
    if ((channelA_ != channelB_) && (repeatState_ == 0))
    {
        displayCount_ += 1.0f;
        controllerCount_ += 1.0f;

        repeatState_ = 1;
    }

    // Original:
    //
    // if (s1 == s2 && r == 1)
    //
    if ((channelA_ == channelB_) && (repeatState_ == 1))
    {
        displayCount_ += 1.0f;
        controllerCount_ += 1.0f;

        repeatState_ = 0;
    }
}


void BuiltinEncoder::updateDirection()
{
    // Original BaseCodeOne direction logic

    if ((channelA_ == HIGH) &&
        (channelB_ == HIGH) &&
        (previousChannelB_ == LOW))
    {
        directionCounter_++;
    }

    if ((channelA_ == LOW) &&
        (channelB_ == LOW) &&
        (previousChannelB_ == HIGH))
    {
        directionCounter_++;
    }

    previousChannelB_ = channelB_;

    if (directionCounter_ > 100)
    {
        direction_ = 0;
    }

    if (directionCounter_ < 20)
    {
        direction_ = 1;
    }
}


float BuiltinEncoder::getControllerRPMAndReset()
{
    // Original:
    //
    // rpmm = (s_2 / (2 * 114)) * 600;
    //
    // 100 ms measurement:
    // 600 = 60 sec/min / 0.1 sec

    float rpm =
        (controllerCount_ / cfg::BUILTIN_COUNTS_PER_REV)
        * 600.0f;

    controllerCount_ = 0.0f;

    return rpm;
}


float BuiltinEncoder::getDisplayRPM() const
{
    // Original:
    //
    // (s / 228) * 12
    //
    // 12 = 60 sec/min / 5 sec

    return
        (displayCount_ / cfg::BUILTIN_COUNTS_PER_REV)
        * 12.0f;
}


int BuiltinEncoder::getDirection() const
{
    return direction_;
}


void BuiltinEncoder::resetDisplayWindow()
{
    displayCount_ = 0.0f;
    directionCounter_ = 0;
}
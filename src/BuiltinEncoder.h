#pragma once

class BuiltinEncoder
{
public:
    void begin();

    // Equivalent to the first encoder-reading
    // section of BaseCodeOne
    void updateCounts();

    // Equivalent to direction detection
    // section of BaseCodeOne
    void updateDirection();

    // RPM used by PI controller every ~100 ms
    float getControllerRPMAndReset();

    // RPM displayed every 5 seconds
    float getDisplayRPM() const;

    // CW / CCW result from built-in encoder
    int getDirection() const;

    // Reset counters after 5-second display
    void resetDisplayWindow();

private:
    float displayCount_ = 0.0f;
    float controllerCount_ = 0.0f;

    int channelA_ = 0;
    int channelB_ = 0;

    int repeatState_ = 0;
    int previousChannelB_ = 0;

    int directionCounter_ = 0;
    int direction_ = 0;
};
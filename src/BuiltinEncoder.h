#pragma once

class BuiltinEncoder
{
public:
    void update();

    float getCounts() const;

    float gettotal() const;

    float getAngleDegrees() const;

    void reset();

    float getRPM(float elapsed) const;

private:
    float counts_ = 0;
    float totalcount_ = 0;

    int channel1_ = 0;
    int channel2_ = 0;

    int state_ = 0;
};
#pragma once

class Sensor
{
public:
    void update();

    const float* getValue() const;

    void reset();

private:
    float values_[6] = {0};
};
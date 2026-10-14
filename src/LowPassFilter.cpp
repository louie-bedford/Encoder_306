#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(float alpha)
{
    alpha_ = alpha;
    filteredValue_ = 0.0f;
    initialized_ = false;
}

float LowPassFilter::update(float input)
{
    if (!initialized_)
    {
        filteredValue_ = input;
        initialized_ = true;
        return filteredValue_;
    }

    filteredValue_ =
        filteredValue_ + alpha_ * (input - filteredValue_);

    return filteredValue_;
}

void LowPassFilter::reset()
{
    filteredValue_ = 0.0f;
    initialized_ = false;
}
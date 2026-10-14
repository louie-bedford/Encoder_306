#pragma once

class LowPassFilter
{
public:
    LowPassFilter(float alpha);

    float update(float input);
    void reset();

private:
    float alpha_;
    float filteredValue_;
    bool initialized_;
};
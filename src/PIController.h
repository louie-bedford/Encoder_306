#pragma once

class PIController
{
public:
    PIController(float kp, float ki);

    float update(float targetRPM, float currentRPM,
                           float elapsedMs);
    void resetIntegral();

private:
    float kp_;
    float ki_;

    float integral_ = 0.0f;
};
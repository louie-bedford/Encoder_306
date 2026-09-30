#pragma once

class PIController
{
public:
    PIController(float kp, float ki);

    bool update(
        unsigned long currentTime,
        float targetRPM,
        float measuredRPM,
        float& controlOutput
    );

private:
    float kp_;
    float ki_;

    float integralError_ = 0.0f;

    int repeatCondition_ = 1;
};
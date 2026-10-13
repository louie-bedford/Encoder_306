#include "PIController.h"
#include <Arduino.h>



PIController::PIController(float kp, float ki)
    : kp_(kp),
      ki_(ki)
{
}


float PIController::update(float targetRPM, float currentRPM,
                           float elapsedMs)
{
    if (elapsedMs <= 0.0f)
        return 0.0f;

    const float dt = elapsedMs / 1000.0f;
    const float error = targetRPM - currentRPM;

    const float candidateIntegral = integral_ + error * dt;
    const float candidateOutput =
        kp_ * error + ki_ * candidateIntegral;

    // Prevent the integral from pushing farther into saturation.
    const bool saturatingHigh =
        candidateOutput > 255.0f && error > 0.0f;
    const bool saturatingLow =
        candidateOutput < 0.0f && error < 0.0f;

    if (!saturatingHigh && !saturatingLow)
        integral_ = candidateIntegral;

    const float output = kp_ * error + ki_ * integral_;
    return constrain(output, 0.0f, 255.0f);
}


void PIController::resetIntegral()
{
    integral_ = 0.0f;
}
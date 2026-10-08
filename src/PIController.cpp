#include "PIController.h"


PIController::PIController(float kp, float ki)
    : kp_(kp),
      ki_(ki)
{
}


float PIController::update(float targetRPM, float currentRPM, float dt)
{
    const float error = targetRPM - currentRPM;

    integral_ = integral_ + error * dt / 1000;

    return kp_ * error + ki_ * integral_;
}


void PIController::resetIntegral()
{
    integral_ = 0.0f;
}
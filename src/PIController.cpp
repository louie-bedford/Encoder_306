#include "PIController.h"


PIController::PIController(float kp, float ki)
    : kp_(kp),
      ki_(ki)
{
}


float PIController::update(float targetDegree, float currentDegree)
{
    const float error = targetDegree - currentDegree;

    integral_ = integral_ + error;

    return kp_ * error + ki_ * integral_;
}


void PIController::resetIntegral()
{
    integral_ = 0.0f;
}
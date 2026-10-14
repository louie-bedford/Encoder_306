#include "PIController.h"
#include <Arduino.h>



PIController::PIController(float kp, float ki)
    : kp_(kp),
      ki_(ki)
{
}


float PIController::update(float targetRPM, float currentRPM,
                           float elapsed)
{
    if (elapsed <= 0.0f)
        return 0.0f;

    float dt = elapsed / 1000.0f;

    float error = targetRPM - currentRPM;

    float integral_temp = integral_ + error * dt;
    
    float output_temp = kp_ * error + ki_ * integral_temp;

    if (!(output_temp > 255.0f && error > 0.0f) && !(output_temp < 0.0f && error < 0.0f))
        integral_ = integral_temp;

    float output = kp_ * error + ki_ * integral_;

    return constrain(output, 0.0f, 255.0f);
}


void PIController::resetIntegral()
{
    integral_ = 0.0f;
}
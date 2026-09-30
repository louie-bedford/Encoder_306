#include "PIController.h"
#include "Config.h"

PIController::PIController(float kp, float ki)
    : kp_(kp),
      ki_(ki)
{
}


bool PIController::update(
    unsigned long currentTime,
    float targetRPM,
    float measuredRPM,
    float& controlOutput
)
{
    // Original:
    //
    // if (b % 13 == 0 && repc == 1)

    if ((currentTime % cfg::PI_PERIOD_MS == 0) &&
        (repeatCondition_ == 1))
    {
        const float error =
            targetRPM - measuredRPM;

        integralError_ =
            ki_ * error
            + integralError_;

        controlOutput =
            cfg::BASE_PWM
            + kp_ * error
            + integralError_;

        repeatCondition_ = 0;

        return true;
    }

    // Original:
    //
    // if (b % 13 == 1)
    //
    if (currentTime % cfg::PI_PERIOD_MS == 1)
    {
        repeatCondition_ = 1;
    }

    return false;
}
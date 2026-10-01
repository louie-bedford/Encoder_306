#pragma once

class MotorDriver
{
public:
    void setDirectionFromDegree(float degree);

    void setPWM(float pwm);

    void stop();
};
#pragma once

class MotorDriver
{
public:
    void begin();

    void setDirectionFromRPM(int commandedRPM);
    void setPWM(float pwm);

    void stop();
};
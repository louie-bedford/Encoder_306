#pragma once


class Quad
{
public:
    void update();

    float getcurrentRPM(float elasped) const;

    float getcurrentRPM_rolling(float elasped) const;

    int getCounts() const;

    int gettotal() const;

    void reset();

    int getDirection() const;


    

private:
    int counts_;
    int totalcounts_;

    float IR1;
    float IR2;
    int state_ = 0;

    int direction_ = 0;   // +1 = CW, -1 = CCW
    int prevIR1_ = 0;
    int prevIR2_ = 0;
};
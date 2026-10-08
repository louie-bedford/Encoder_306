#include <Arduino.h>

#include "Quad.h"
#include "config.h"


void Quad::update()
{
    IR1 = (analogRead(cfg::IR_5) > 300) ? 1.0f : 0.0f;
    IR2 = (analogRead(cfg::IR_6) > 300) ? 1.0f : 0.0f;

    if (IR1 != IR2  && state_ == 0)
    {
        counts_ = counts_ + 1;
        totalcounts_+=1;
        state_ = 1;
    }


    if (IR1 == IR2 && state_ == 1)
    {
        counts_ = counts_ + 1;
        totalcounts_+=1;
        state_ = 0;
    }

     // IR1 rises first
    if (IR1 == 1 && prevIR1_ == 0)
    {
        if (IR2 == 0)
        {
            direction_ = 1;   // CW
        }
        else
        {
            direction_ = -1;  // CCW
        }

        counts_++;
        totalcounts_++;
    }


    // IR2 rises first
    if (IR2 == 1 && prevIR2_ == 0)
    {
        if (IR1 == 0)
        {
            direction_ = -1;  // CCW
        }
        else
        {
            direction_ = 1;   // CW
        }

        counts_++;
        totalcounts_++;
    }


    prevIR1_ = IR1;
    prevIR2_ = IR2;
}

float Quad::getcurrentRPM(float elasped) const // currently usig 10ms for updating
{
    if (elasped <= 0.0f)
        return 0.0f;

    return counts_ * 60000.0f /
           (cfg::IR_COUNTS_PER_REV * elasped);
}

int Quad::getCounts() const
{
    return counts_;
}

int Quad::gettotal() const
{
    return totalcounts_;
}


void Quad::reset()
{
    counts_ = 0;
}

int Quad::getDirection() const
{
    return direction_;
}
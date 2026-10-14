#include <Arduino.h>

#include "Quad.h"
#include "config.h"


void Quad::update(float now)
{
    IR1 = (analogRead(cfg::IR_5) > 280) ? 1.0f : 0.0f;
    IR2 = (analogRead(cfg::IR_6) > 280) ? 1.0f : 0.0f;

    rawIRA_ = analogRead(cfg::IR_5);
    rawIRB_ = analogRead(cfg::IR_6);

    filteredIRA_ = filterA_.update(rawIRA_);
    filteredIRB_ = filterB_.update(rawIRB_);

    if (IR1 != IR2  && state_ == 0)
    {
        counts_ = counts_ + 1;
        if(now > 5000){
            totalcounts_+=1;
        }
        state_ = 1;
    }


    if (IR1 == IR2 && state_ == 1)
    {
        counts_ = counts_ + 1;
        if(now > 5000){
            totalcounts_+=1;
        }
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

float Quad::getcurrentRPM_rolling(float elapsed, float now)
{
    // Fill the initial six history samples
    if (timeHistory[5] == 0)
    {
        countHistory[index] = totalcounts_;
        timeHistory[index] = now;
        index++;

        // First sample: use counts accumulated since startup
        if (index == 1)
        {
            if (timeHistory[0] <= 0)
                return 0.0f;

            return totalcounts_ * 60000.0f /
                   (cfg::IR_COUNTS_PER_REV * timeHistory[0]);
        }

        // All six history slots are now filled
        if (index == 6)
        {
            index = 0;
        }

        return (totalcounts_ - countHistory[0]) * 60000.0f /
               (cfg::IR_COUNTS_PER_REV * (now - timeHistory[0]));
    }

    // Replace the current history slot with the newest sample
    countHistory[index] = totalcounts_;
    timeHistory[index] = now;

    // Calculate over approximately 500 ms
    if (index == 5)
    {
        time_interval =
            timeHistory[index] - timeHistory[0];

        count_interval =
            countHistory[index] - countHistory[0];

        index = 0;
    }
    else
    {
        time_interval =
            timeHistory[index] - timeHistory[index + 1];

        count_interval =
            countHistory[index] - countHistory[index + 1];

        index++;
    }

    if (time_interval <= 0)
        return 0.0f;

    return count_interval * 60000.0f /
           (cfg::IR_COUNTS_PER_REV * time_interval);
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

float Quad::getRawA() const
{
    return rawIRA_;
}

float Quad::getRawB() const
{
    return rawIRB_;
}

float Quad::getFilteredA() const
{
    return filteredIRA_;
}

float Quad::getFilteredB() const
{
    return filteredIRB_;
}
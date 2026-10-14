#pragma once
#include "LowPassFilter.h"


class Quad
{
public:
    void update(float now);

    float getcurrentRPM(float elasped) const;

    float getcurrentRPM_rolling(float elapsed,float now);

    int getCounts() const;

    int gettotal() const;

    void reset();

    int getDirection() const;


    float getRawA() const;
    float getRawB() const;

    float getFilteredA() const;
    float getFilteredB() const;

    

private:
    int counts_;
    int totalcounts_;
    unsigned long countHistory[6] = {0};
    unsigned long timeHistory[6] = {0};
    int index = 0;
    float count_interval;
    float time_interval;

    float IR1;
    float IR2;
    int state_ = 0;

    //-------------for filter
    LowPassFilter filterA_{0.3f};
    LowPassFilter filterB_{0.3f};

    float rawIRA_;
    float rawIRB_;

    float filteredIRA_;
    float filteredIRB_;
    //-------------------

    int direction_ = 0;   // +1 = CW, -1 = CCW
    int prevIR1_ = 0;
    int prevIR2_ = 0;
};
#include <Arduino.h>

#include "Sensor.h"
#include "config.h"


void Sensor::update()
{
    Serial.println("--------------------------");

    Serial.print("IR1: ");
    Serial.println(analogRead(cfg::IR_1));

    Serial.print("IR2: ");
    Serial.println(analogRead(cfg::IR_2));

    Serial.print("IR3: ");
    Serial.println(analogRead(cfg::IR_3));

    Serial.print("IR4: ");
    Serial.println(analogRead(cfg::IR_4));

    Serial.print("IR5: ");
    Serial.println(analogRead(cfg::IR_5));

    Serial.print("IR6: ");
    Serial.println(analogRead(cfg::IR_6));



    values_[0] = (analogRead(cfg::IR_1) > 180) ? 1.0f : 0.0f; // the range is arbitary now
    values_[1] = (analogRead(cfg::IR_2) > 180) ? 1.0f : 0.0f;
    values_[2] = (analogRead(cfg::IR_3) > 180) ? 1.0f : 0.0f;
    values_[3] = (analogRead(cfg::IR_4) > 180) ? 1.0f : 0.0f;
    values_[4] = (analogRead(cfg::IR_5) > 180) ? 1.0f : 0.0f;
    values_[5] = (analogRead(cfg::IR_6) > 180) ? 1.0f : 0.0f;
}


const float* Sensor::getValue() const
{
    return values_;
}


void Sensor::reset()
{
    for (int i = 0; i < 6; i++)
    {
        values_[i] = 0;
    }
}
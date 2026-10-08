#include <Arduino.h>

#include "Sensor.h"
#include "config.h"
#include "BuiltinEncoder.h"
#include "PIController.h"
#include "Motordriver.h"
#include "Quad.h"


Sensor sensor;
BuiltinEncoder Encoder;
PIController PIcontroller(0.4f, 0.0f);
MotorDriver Motor;
Quad IRQuad;

//PIcontroller = PIController::PIController(10, cfg::KI);

unsigned long lastSensorUpdate = 0;
float time_variant = 0;
float RPM_B = 0;
float RPM = 0;
float target = 70;
float power = 0;


void setup()
{
    Serial.begin(230400);
    Encoder.reset();
    IRQuad.reset();
    PIcontroller.resetIntegral();

}


void loop()
{
    const unsigned long now = millis();

    Encoder.update();
    IRQuad.update();

    time_variant = now - lastSensorUpdate;

    if (time_variant >= 100)
    {
        lastSensorUpdate = now;

        //-------------------------------------------
        sensor.update();

        const float* values = sensor.getValue();


        Serial.print("[");

        for (int i = 0; i < 6; i++)
        {
            Serial.print(static_cast<int>(values[i]));

            if (i < 5)
            {
                Serial.print(", ");
            }
        }

        Serial.println("]");
        //------------------------------------------------
        int direction = IRQuad.getDirection();

        if (direction == 1)
        {
            Serial.println("CW");
        }
        else if (direction == -1)
        {
            Serial.println("CCW");
        }

        Serial.println("Spontaneous speed from builtin encoder : ");
        RPM_B = Encoder.getRPM(time_variant);
        Serial.print(RPM_B);

        Serial.println("Spontaneous speed from IR encoder : ");
        RPM = IRQuad.getcurrentRPM(time_variant);
        Serial.print(RPM);
        Serial.println("Built-in counts: ");
        Serial.println(Encoder.gettotal());

        Serial.println("IR counts: ");
        Serial.println(IRQuad.gettotal());

        power = PIcontroller.update(target, RPM_B, time_variant);

        power = constrain(power, 0.0f, 255.0f);
        Motor.setPWM(0);
        Encoder.reset();
        IRQuad.reset();
    }

    if (now >= 20000)
    {
      Motor.setPWM(0);
      PIcontroller.resetIntegral();     
      Encoder.getCounts();

      Serial.println("-------------------------");
      Serial.println("The average RPM from buildin encoder : ");
      float RPMT = Encoder.gettotal()*60000 / (cfg::ENCODER_COUNTS_PER_REV*now);
      Serial.print(RPMT);
      Serial.println("The average RPM from IR encoder : ");
      float RPMF = (float)IRQuad.gettotal()*60000 / (cfg::IR_COUNTS_PER_REV*now);
      Serial.print(RPMF);

      delay(100000);
    }
}


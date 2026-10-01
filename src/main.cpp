#include <Arduino.h>

#include "BuiltinEncoder.h"
#include "MotorDriver.h"
#include "PIController.h"
#include "config.h"
#include "Sensor.h"


// =====================================================================
// Global objects
// =====================================================================

MotorDriver motor;
BuiltinEncoder builtinEncoder;
Sensor sensor;



// =====================================================================
// BaseCode variables
// =====================================================================

unsigned long lastSensorUpdate = 0;

float deg = 45.0f;


// Keep the original BaseCode behaviour.
// kp is calculated using the initial deg = 45 before setup().
float kp = 0.6f * 90.0f / deg;

PIController controller(kp, cfg::KI);


int t = 0;
int t0 = 0;

int finish = 0;
int rep = 1;


// =====================================================================
// Setup
// =====================================================================

void setup()
{
    Serial.begin(cfg::SERIAL_BAUD);

    Serial.println("Enter the desired rotation in degree.");


    while (Serial.available() == 0)
    {
        // Wait for user input
    }


    deg = Serial.readString().toFloat();


    motor.setDirectionFromDegree(deg);


    deg = abs(deg);
}


// =====================================================================
// Main loop
// =====================================================================

void loop()
{
    t = millis();

    t0 = t;


    // Run each positioning operation for 4 seconds,
    // for a maximum of 10 repetitions.
    while (t < t0 + cfg::POSITION_TIME_MS &&
           rep <= cfg::MAX_REPETITIONS)
    {

        if (t - lastSensorUpdate >= 10){
            lastSensorUpdate = t;

            sensor.update();

            const float* values = sensor.getValue();

            Serial.print("[");

            for (int i = 0; i <6; i++)
            {
                Serial.print(static_cast<int>(values[i]));

                if (i < 5)
                {
                    Serial.print(", ");
                }
            }

            Serial.println("]");
        }

        // Run the PI controller every 10 ms.
        if (t % 10 == 0)
        {
            const float counts =
                builtinEncoder.getCounts();

            const float targetCounts =
                deg * cfg::ENCODER_COUNTS_PER_REV / 360.0f;

                
            if (counts < targetCounts)
            {
                const float currentDegree =
                    builtinEncoder.getAngleDegrees();

                const float pwm =
                    controller.update(
                        deg,
                        currentDegree
                    );

                motor.setPWM(pwm);
            }


            if (counts >= targetCounts)
            {
                motor.stop();

                controller.resetIntegral();
            }


            delay(1);
        }


        // Read and count the built-in encoder.
        builtinEncoder.update();


        // Update time.
        t = millis();

        finish = 1;
    }


    // =================================================================
    // Display results
    // =================================================================

    if (finish == 1)
    {
        delay(500);

        rep = rep + 1;


        Serial.print(
            "shaft possition from optical absolute sensor from home position: "
        );

        Serial.println(0);


        Serial.print(
            "shaft displacement from optical absolute sensor: "
        );

        Serial.println(0);


        Serial.print(
            "Shaft displacement from motor's builtin encoder: "
        );

        const float builtinAngle =
            builtinEncoder.getAngleDegrees();

        Serial.println(builtinAngle);


        const float Error =
            0 - builtinAngle;

        Serial.print("Error :");

        Serial.println(Error);

        Serial.println();


        builtinEncoder.reset();

        finish = 0;
    }


    motor.stop();
} 
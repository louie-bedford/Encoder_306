#include <Arduino.h>

#include "Sensor.h"
#include "config.h"
#include "BuiltinEncoder.h"
#include "PIController.h"
#include "Motordriver.h"
#include "Quad.h"


Sensor sensor;
BuiltinEncoder Encoder;
PIController PIcontroller(0.8f, cfg::KI);
MotorDriver Motor;
Quad IRQuad;


// Timing variables
unsigned long lastSensorUpdate = 0;
unsigned long lastCSV = 0;

float time_variant = 0;


// Speed and control variables
float RPM_B = 0;
float RPM = 0;
float RPM_rolling = 0.0f;
float target = 250;
float power = 0;


void setup()
{
    Serial.begin(230400);

    Encoder.reset();
    IRQuad.reset();
    PIcontroller.resetIntegral();

    // CSV header for LPF waveform comparison.
    // Raw and filtered IR signals will be plotted together in MATLAB.
    Serial.println(
        "time_ms,rawA,filtA,rawB,filtB,"
        "builtinRPM,irRPM100,irRPM500,pwm"
    );
}


void loop()
{
    const unsigned long now = millis();

    /*
     * Encoder sampling runs continuously.
     *
     * IRQuad.update() reads the raw IR channels and also updates
     * the low-pass filtered values.
     *
     * The quadrature decoder currently still uses the raw signal.
     * The filtered signal is only recorded for comparison.
     */
    Encoder.update();
    IRQuad.update();


    time_variant = now - lastSensorUpdate;


    /*
     * LPF waveform logging
     *
     * Log approximately every 2 ms so the raw and filtered
     * optical encoder waveforms can be compared in MATLAB.
     *
     * CSV format:
     * time_ms, rawA, filtA, rawB, filtB
     */
    if (now - lastCSV >= 2)
    {
        lastCSV = now;

        Serial.print(now);
        Serial.print(",");
        Serial.print(IRQuad.getRawA());
        Serial.print(",");
        Serial.print(IRQuad.getFilteredA());
        Serial.print(",");
        Serial.print(IRQuad.getRawB());
        Serial.print(",");
        Serial.print(IRQuad.getFilteredB());
        Serial.print(",");
        Serial.print(RPM_B);
        Serial.print(",");
        Serial.print(RPM);
        Serial.print(",");
        Serial.print(RPM_rolling);
        Serial.print(",");
        Serial.println(power);

        // End each row as a MATLAB matrix row
        Serial.println(";");
    }


    /*
     * Speed estimation and motor control update.
     *
     * This section remains at approximately 100 ms intervals.
     * It is independent of the faster LPF waveform logging.
     */
    if (time_variant >= 100)
    {
        lastSensorUpdate = now;


        /*
         * Full six-channel sensor diagnostic output is disabled
         * during the LPF test to avoid unnecessary Serial traffic.
         */

        /*
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
        */


        /*
         * Direction diagnostic output is also disabled so the
         * Serial stream remains valid CSV data.
         */

        int direction = IRQuad.getDirection();

        /*
        if (direction == 1)
        {
            Serial.println("CW");
        }
        else if (direction == -1)
        {
            Serial.println("CCW");
        }
        */


        // Built-in encoder RPM over the current 100 ms window.
        RPM_B = Encoder.getRPM(time_variant);
        Encoder.reset();


        // IR encoder RPM over the current 100 ms window.
        RPM = IRQuad.getcurrentRPM(time_variant);
        RPM_rolling = IRQuad.getcurrentRPM_rolling(time_variant, now);
        IRQuad.reset();


        /*
         * PI speed control uses the built-in encoder measurement.
         */
        power = PIcontroller.update(target, RPM_B, time_variant);
        Motor.setPWM(power);
    }


    /*
     * Stop the test after 20 seconds.
     *
     * Text-based summary output is disabled for this LPF test
     * so the captured Serial data remains valid CSV.
     */
    if (now >= 20000)
    {
        Motor.setPWM(0);
        PIcontroller.resetIntegral();

        /*
        Encoder.getCounts();

        Serial.println("-------------------------");
        Serial.println("The average RPM from builtin encoder : ");

        float RPMT =
            Encoder.gettotal() * 60000.0f /
            (cfg::ENCODER_COUNTS_PER_REV * now);

        Serial.println(RPMT);

        Serial.println("The average RPM from IR encoder : ");

        float RPMF =
            (float)IRQuad.gettotal() * 60000.0f /
            (cfg::IR_COUNTS_PER_REV * now);

        Serial.println(RPMF);
        */

        // Hold the program after the test.
        delay(100000);
    }
}
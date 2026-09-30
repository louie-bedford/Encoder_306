#include "config.h"
#include "MotorDrive.h"
#include "BuiltinEncoder.h"
#include "PIController.h"


MotorDriver motor;
BuiltinEncoder builtinEncoder;

PIController speedController(
    cfg::KP,
    cfg::KI
);


// -------------------------------------------------
// BaseCodeOne variables
// -------------------------------------------------

int commandedRPM = 0;

// Built-in encoder RPM used by PI controller
float measuredRPM = 0.0f;

// PI output
float controlOutput = 0.0f;


// Timing
unsigned long currentTime = 0;
unsigned long startTime = 0;
unsigned long displayStartTime = 0;


// Equivalent to original repeat variable
bool displayStartStored = false;


// Main-loop exit condition
bool finished = false;



void setup()
{
    Serial.begin(cfg::SERIAL_BAUD);

    motor.begin();
    builtinEncoder.begin();


    Serial.println("Enter the desired RPM.");


    while (Serial.available() == 0)
    {
        // Wait for user input
    }


    commandedRPM =
        Serial.readString().toFloat();


    // Set motor direction using original sign
    motor.setDirectionFromRPM(commandedRPM);


    // Controller always uses positive RPM magnitude
    commandedRPM = abs(commandedRPM);
}



void loop()
{
    currentTime = millis();

    startTime = currentTime;


    // -------------------------------------------------
    // Original BaseCodeOne 15 s run
    // -------------------------------------------------

    while (
        (currentTime >= startTime) &&
        (currentTime <= startTime + cfg::RUN_TIME_MS) &&
        !finished
    )
    {

        // =============================================
        // 1. PI CONTROLLER
        // =============================================

        if (
            speedController.update(
                currentTime,
                commandedRPM,
                measuredRPM,
                controlOutput
            )
        )
        {
            motor.setPWM(controlOutput);
        }



        // =============================================
        // 2. BUILT-IN ENCODER COUNTS
        // =============================================

        builtinEncoder.updateCounts();



        // =============================================
        // 3. TIMING / DISPLAY
        // =============================================

        currentTime = millis();


        if (
            (currentTime % cfg::SPEED_SAMPLE_MS <= 1) &&
            !displayStartStored
        )
        {
            displayStartTime = currentTime;
            displayStartStored = true;
        }


        // -------------------------------------------------
        // 100 ms built-in encoder RPM
        // -------------------------------------------------

        if (currentTime % cfg::SPEED_SAMPLE_MS == 0)
        {
            Serial.print("time in ms: ");
            Serial.print(
                currentTime - displayStartTime
            );


            Serial.print(
                "  spontaneous speed from builtin encoder:  "
            );


            measuredRPM =
                builtinEncoder.getControllerRPMAndReset();


            Serial.println(measuredRPM);



            // =============================================
            // 4. 5 SECOND DISPLAY
            // =============================================

            if (
                (currentTime - displayStartTime)
                % cfg::DISPLAY_PERIOD_MS
                == 0
            )
            {
                float builtinDisplayRPM =
                    builtinEncoder.getDisplayRPM();


                Serial.println();


                Serial.print(
                    "RPM from builtin encoder: "
                );

                Serial.println(
                    builtinDisplayRPM
                );


                // =========================================
                // TODO:
                // Our optical quadrature encoder
                // =========================================

                float opticalEncoderRPM = 0.0f;


                Serial.print(
                    "RPM from optical quadrature encoder: "
                );

                Serial.println(
                    opticalEncoderRPM
                );


                // Project definition:
                // optical encoder - built-in encoder

                float error =
                    opticalEncoderRPM
                    - builtinDisplayRPM;


                Serial.print("Error: ");
                Serial.println(error);



                // =========================================
                // BUILT-IN ENCODER DIRECTION
                // =========================================

                Serial.print(
                    "direction read by motor's sensor: "
                );


                if (
                    builtinEncoder.getDirection() == 0
                )
                {
                    Serial.print("CW");
                }
                else
                {
                    Serial.print("CCW");
                }


                Serial.print("  ,   ");



                // =========================================
                // TODO:
                // Optical encoder direction
                // =========================================

                Serial.print(
                    "direction read by sensor:  "
                );

                Serial.println("");


                Serial.println();


                builtinEncoder.resetDisplayWindow();
            }


            delay(1);
        }



        // =============================================
        // 5. BUILT-IN ENCODER DIRECTION DETECTION
        // =============================================

        builtinEncoder.updateDirection();



        // =============================================
        // 6. UPDATE TIME
        // =============================================

        currentTime = millis();
    }



    // -------------------------------------------------
    // End of 15-second test
    // -------------------------------------------------

    motor.stop();

    finished = true;
}

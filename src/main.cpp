#include <Arduino.h>
#include "encoder.h"

#define BAUD_RATE 11520

void setup() {
  Serial.begin(BAUD_RATE);
  encoderSetup();
}

void loop() {
  encoderLoop();
}
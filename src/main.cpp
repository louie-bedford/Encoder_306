#include <Arduino.h>
#include "abs_encoder.h"

#define BAUD_RATE 11520
#define MODE 0 // 0 for absolute mode, 1 for quadrature mode

void setup() {
  Serial.begin(BAUD_RATE);
  if (MODE == 0) {
    absEncoderSetup();
  }
}

void loop() {
  if (MODE == 0) {
    absEncoderLoop();
  }
}
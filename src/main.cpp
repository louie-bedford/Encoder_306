#include <Arduino.h>
#include "abs_encoder.h"
#include "quadrature_encoder.h"

#define BAUD_RATE 11520
#define MODE 1 // 0 for absolute mode, 1 for quadrature mode

void setup() {
  Serial.begin(BAUD_RATE);

  if (MODE == 0) {
    absEncoderSetup();
  } else if (MODE == 1) {
    quadratureEncoderSetup();
  }
}

void loop() {
  if (MODE == 0) {
    absEncoderLoop();
  } else if (MODE == 1) {
    quadratureEncoderLoop();
  }
}
#include <Arduino.h>
#include "encoder.h"

namespace {
const uint8_t ledPins[] = {2, 3, 4, 5, 6};
const uint8_t photoTransistorPins[] = {A0, A1, A2, A3, A4};
const size_t encoderBitCount = 5;
const int signalThreshold = 350;

int encoderReadings[encoderBitCount] = {0};
int encoderBits[encoderBitCount] = {0};
int encoderValue = 0;
float angle = 0.0;
}  // namespace

void encoderSetup() {
  for (size_t bit = 0; bit < encoderBitCount; ++bit) {
    pinMode(ledPins[bit], OUTPUT);
    digitalWrite(ledPins[bit], HIGH);
    pinMode(photoTransistorPins[bit], INPUT);
  }
}

void encoderLoop() {
  for (size_t i = 0; i < encoderBitCount; ++i) {
    encoderReadings[i] = analogRead(photoTransistorPins[i]);
  }

  encoderValue = 0;
  angle = 0.0;

  for (size_t i = 0; i < encoderBitCount; ++i) {
    encoderBits[i] = encoderReadings[i] > signalThreshold ? 1 : 0;
    encoderValue = (encoderValue << 1) | encoderBits[i];
  }

  for (size_t i = 0; i < encoderBitCount; ++i) {
    Serial.print("bit");
    Serial.print(i);
    Serial.print(":");
    Serial.print(encoderBits[i]);
    if (i < encoderBitCount - 1) {
      Serial.print("\t");
    }
  }
  Serial.println();

  angle = (encoderValue / 31.0) * 360.0;
}
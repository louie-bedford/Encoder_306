#include <Arduino.h>
#include "quadrature_encoder.h"

namespace {
const uint8_t channelAPin = 2;
const uint8_t channelBPin = 3;
const uint16_t pulsesPerRevolution = 20;  // Update to match your encoder disk/slots.

volatile unsigned long lastEdgeMicros = 0;
volatile unsigned long lastPeriodMicros = 0;
volatile int8_t direction = 0;
volatile float rpm = 0.0f;
volatile uint8_t lastChannel = 0;

void handleQuadratureInterrupt(uint8_t channel) {
  const unsigned long nowMicros = micros();

  if (lastEdgeMicros != 0) {
    const unsigned long deltaMicros = nowMicros - lastEdgeMicros;

    if (deltaMicros > 1000UL) {
      lastPeriodMicros = deltaMicros;

      if (channel == 0 && lastChannel == 1) {
        direction = 1;
      } else if (channel == 1 && lastChannel == 0) {
        direction = -1;
      }

      const float periodSeconds = static_cast<float>(deltaMicros) / 1000000.0f;
      if (periodSeconds > 0.0f) {
        rpm = 60.0f / (periodSeconds * static_cast<float>(pulsesPerRevolution));
      }
    }
  }

  lastChannel = channel;
  lastEdgeMicros = nowMicros;
}

void handleChannelA() {
  handleQuadratureInterrupt(0);
}

void handleChannelB() {
  handleQuadratureInterrupt(1);
}
}  // namespace

void quadratureEncoderSetup() {
  pinMode(channelAPin, INPUT);
  pinMode(channelBPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(channelAPin), handleChannelA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(channelBPin), handleChannelB, CHANGE);

  lastEdgeMicros = 0;
  lastPeriodMicros = 0;
  direction = 0;
  rpm = 0.0f;
  lastChannel = 0;
}

void quadratureEncoderLoop() {
  static unsigned long lastPrintMillis = 0;

  if (millis() - lastPrintMillis >= 100UL) {
    lastPrintMillis = millis();

    Serial.print("Quadrature RPM: ");
    Serial.print(quadratureEncoderGetRpm(), 2);
    Serial.print(" | Direction: ");
    if (quadratureEncoderGetDirection() > 0) {
      Serial.println("CW");
    } else if (quadratureEncoderGetDirection() < 0) {
      Serial.println("CCW");
    } else {
      Serial.println("STOP");
    }
  }
}

float quadratureEncoderGetRpm() {
  return rpm;
}

int8_t quadratureEncoderGetDirection() {
  return direction;
}

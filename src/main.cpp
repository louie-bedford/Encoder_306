#include <Arduino.h>

#define BAUD_RATE 11520

// Define the pins for the LEDs and photo transistors
const uint8_t ledPins[] = {2, 3, 4, 5, 6};
const uint8_t photoTransistorPins[] = {A0, A1, A2, A3, A4};

// Define the number of bits in the encoder and the threshold for detecting a
// signal
const size_t encoderBitCount = 5;
const int signalThreshold = 350;

// Define arrays to hold the readings from the photo transistors and the
// corresponding bits
int encoder_readings[5] = {0, 0, 0, 0, 0};
int encoder_bits[5] = {0, 0, 0, 0, 0};
int encoder_value = 0;
float angle = 0.0;

void setup() {
  // Initialize serial communication
  Serial.begin(BAUD_RATE);

  // Set the LED pins as OUTPUT and the photo transistor pins as INPUT
  for (size_t bit = 0; bit < encoderBitCount; ++bit) {
    pinMode(ledPins[bit], OUTPUT);
    digitalWrite(ledPins[bit], HIGH);
    pinMode(photoTransistorPins[bit], INPUT);
  }
}

void loop() {
  // Read the state of the photo transistors and store them in the encoder_bits
  // array
  for (size_t i = 0; i < encoderBitCount; i++) {
    encoder_readings[i] = analogRead(photoTransistorPins[i]);
  }

  // Reset encoder_value and angle before calculating the new values
  encoder_value = 0;
  angle = 0.0;

  // Calculate the encoder value based on the bits read from the photo
  // transistors.
  for (size_t i = 0; i < encoderBitCount; ++i) {
    encoder_bits[i] = encoder_readings[i] > signalThreshold ? 1 : 0;
    encoder_value = (encoder_value << 1) | encoder_bits[i];
  }

  // Print five channels for the Arduino Serial Plotter.
  for (size_t i = 0; i < encoderBitCount; ++i) {
    Serial.print("bit");
    Serial.print(i);
    Serial.print(":");
    Serial.print(encoder_bits[i]);
    if (i < encoderBitCount - 1) {
      Serial.print("\t");
    }
  }
  Serial.println();

  // Calculate the angle based on the encoder value.
  angle = (encoder_value / 31.0) * 360.0;
}
#ifndef QUADRATURE_ENCODER_H
#define QUADRATURE_ENCODER_H

#include <Arduino.h>

void quadratureEncoderSetup();
void quadratureEncoderLoop();
float quadratureEncoderGetRpm();
int8_t quadratureEncoderGetDirection();

#endif

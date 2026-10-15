// ============================================================================
// BaseCodeTwo (closed-loop shaft positioning) + optical absolute encoder
// Positive command = CW, negative command = CCW.
// Controller and built-in encoder counting are unchanged from BaseCodeTwo.
// Lines marked [OPT] are the additions for the optical absolute encoder.
// ============================================================================

// ---------------------------------------------------------------------------
// [OPT] Optical absolute encoder settings
// ---------------------------------------------------------------------------
const uint8_t absPins[] = {A0, A1, A2, A3,
                           A4};  // A0 = LSB (outer, finest track) ... A4 = MSB
const int ABS_BITS = 5;          // 5 tracks -> 32 sectors
const int absThreshold[ABS_BITS] = {
    245, 307, 266, 225,
    245};  // per-channel ADC threshold: set to midpoint of measured min/max
const int ABS_SIGN =
    1;  // +1 if disk angle increases when shaft turns CW, -1 if it decreases
const float ABS_SECTOR = 360.0 / 32.0;  // 11.25 deg per sector

float prevAngle =
    0;             // [OPT] optical angle at the end of the previous positioning
int cmdSign = 1;   // [OPT] +1 = CW commanded, -1 = CCW commanded
float absErr[10];  // [OPT] |error| of each of the 10 positionings
bool summaryDone = false;  // [OPT] makes sure the final average is printed once

// [OPT] Absolute disk angle in degrees [0,360), reported at the sector centre
float absReadAngle() {
  int code = 0;
  for (int i = 0; i < ABS_BITS; i++) {
    analogRead(
        absPins[i]);  // dummy read lets the ADC settle (high source impedance)
    int v = analogRead(absPins[i]);
    code |= (v > absThreshold[i] ? 1 : 0)
            << i;  // A0 -> bit 0 (LSB), A4 -> bit 4 (MSB)
  }
  return code * ABS_SECTOR + ABS_SECTOR / 2.0;
}

// [OPT] Signed difference (a - b) wrapped into [-180,180)
float angleDiff(float a, float b) {
  float d = a - b;
  while (d >= 180.0) d -= 360.0;
  while (d < -180.0) d += 360.0;
  return d;
}

// ---------------------------------------------------------------------------
// BaseCodeTwo variables
// ---------------------------------------------------------------------------
float deg = 45;  // Rotation degree
float s = 0;     // Encoder counts
int sm1 = 0;     // Built-in chanel 1
int sm2 = 0;     // Built-in chanel 2
int r =
    0;  // indicator for reading builtin encoder to avoid the reading redundancy
float er;   // Proportional error for PI controller
float eri;  // Integral error for PI controller

int t = 0;   // time in ms
int t0 = 0;  // memory for time in ms

int finish = 0;  // finish indicator
int rep = 1;     // Repetition indicator

void setup() {
  Serial.begin(250000);  // Baud rate of communication

  Serial.println("Enter the desired rotation in degree.");

  while (Serial.available() == 0)  // Obtaining data from user
  {
    // Wait for user input
  }

  deg = Serial.readString()
            .toFloat();  // Reading the Input string from Serial port.
  if (deg < 0) {
    analogWrite(3, 255);  // change the direction of rotation by applying
                          // voltage to pin 3 of arduino
    cmdSign = -1;         // [OPT] remember commanded direction (CCW)
  }
  deg = abs(deg);

  for (int i = 0; i < ABS_BITS; i++) pinMode(absPins[i], INPUT);  // [OPT]
  prevAngle = absReadAngle();  // [OPT] home position of the optical encoder
}

float kp = .6 * 90 / deg;  // proportional gain of PI
float ki = .02;            // integral gain of PI

void loop() {
  // put your main code here, to run repeatedly:

  t = millis();  // reading time
  t0 = t;        // sving the current time in memory
  while (
      t < t0 + 4000 &&
      rep <=
          10)  // let the code to ran for 4 seconds each with repetitions of 10
  {
    if (t % 10 == 0)  // PI controller that runs every 10ms
    {
      if (s < deg * 114 * 2 / 360) {
        er = deg - s * 360 / 228;
        eri = eri + er;
        analogWrite(6, kp * er + ki * eri);
      }

      if (s >= deg * 228 / 360) {
        analogWrite(6, 0);
        eri = 0;
      }
      delay(1);
    }

    sm1 = digitalRead(7);  // reading chanel 1
    sm2 = digitalRead(8);  // reading chanel 2

    if (sm1 != sm2 && r == 0) {  // counting the number changes for both chanels
      s = s + 1;
      r = 1;  // this indicator wont let this condition, (sm1 != sm2), to be
              //
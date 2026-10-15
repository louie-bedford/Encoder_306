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
              // counted until the next condition, (sm1 == sm2), happens
    }
    if (sm1 == sm2 && r == 1) {
      s = s + 1;
      r = 0;  // this indicator wont let this condition, (sm1 == sm2), to be
              // counted until the next condition, (sm1 != sm2), happens
    }

    t = millis();  // updating time
    finish = 1;    // cghanging finish indicator
  }

  if (finish == 1) {  // this part of the code is for displaying the result
    // [OPT] Read the optical encoder first, at the same instant the built-in
    // count stopped. (Done only here, never inside the 4 s loop, so the ADC
    // does not disturb the encoder polling.)
    float pos = absReadAngle();  // absolute angle from the disk's home mark

    delay(500);         // half second delay
    int idx = rep - 1;  // [OPT] index of this positioning (0..9)
    rep = rep + 1;      // increasing the repetition indicator

    // [OPT] signed optical movement: + = CW, - = CCW
    float dSigned = ABS_SIGN * angleDiff(pos, prevAngle);
    // [OPT] optical displacement along the commanded direction (positive when
    // the sensor agrees with the command, like the base code's output)
    float dOpt = cmdSign * dSigned;
    float dBuilt = s * 360 / 228;  // built-in displacement, same as base code
    float err = dOpt - dBuilt;     // [OPT] error = yours - built-in
    prevAngle = pos;               // [OPT] next positioning starts from here
    absErr[idx] = fabs(err);

    Serial.print(
        "shaft possition from optical absolute sensor from home position: ");
    Serial.println(pos);

    Serial.print("shaft displacement from optical absolute sensor: ");
    Serial.println(dOpt);

    Serial.print("Shaft displacement from motor's builtin encoder: ");
    Serial.println(dBuilt);  // every full Revolution of the shaft is associated
                             // with 228 counts of builtin encoder so to turn it
                             // to degre we can use this formula (s * 360 /
                             // 228), "s" is the number of built-in encoder
                             // counts

    Serial.print("Error :");
    Serial.println(err);  // displaying error

    Serial.print("direction read by optical sensor: ");  // [OPT] CW / CCW
    if (dSigned > 0)
      Serial.println("CW");
    else if (dSigned < 0)
      Serial.println("CCW");
    else
      Serial.println("none");
    Serial.println();
    s = 0;
    finish = 0;
  }

  // [OPT] After the 10th positioning: average unsigned error of the 8 best
  // (smallest) of 10
  if (rep > 10 && !summaryDone) {
    float sorted[10];
    for (int i = 0; i < 10; i++) sorted[i] = absErr[i];
    for (int i = 1; i < 10; i++) {  // insertion sort, ascending
      float key = sorted[i];
      int j = i - 1;
      while (j >= 0 && sorted[j] > key) {
        sorted[j + 1] = sorted[j];
        j--;
      }
      sorted[j + 1] = key;
    }
    float sum = 0;
    for (int i = 0; i < 8; i++) sum += sorted[i];
    Serial.print("Average unsigned error, best 8 of 10: ");
    Serial.println(sum / 8.0);
    summaryDone = true;
  }

  analogWrite(6, 0);  // turning off the motor
}
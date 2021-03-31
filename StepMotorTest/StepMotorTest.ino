#include "Servo.h"
#define SERVO_0_PIN 8
#define SERVO_1_PIN 4
#define MAX_ROTATION_STEP 1100

Servo servo0(SERVO_0_PIN, MAX_ROTATION_STEP, false);
Servo servo1(SERVO_1_PIN, MAX_ROTATION_STEP, false);
static bool forward = true;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup: Driving all servos home");
  servo0.driveHome();
  servo1.driveHome();
  servo0.driveSteps(Direction::Forward, MAX_ROTATION_STEP / 2);
  servo1.driveSteps(Direction::Forward, MAX_ROTATION_STEP / 2);
}

void loop() {
  int s = 300;
  Serial.println("Looop");
  Serial.println(forward, DEC);
  if (forward) {
    servo0.driveSteps(Direction::Forward, s);
    servo1.driveSteps(Direction::Forward, s);
  } else {
    servo0.driveSteps(Direction::Backward, s);
    servo1.driveSteps(Direction::Backward, s);
  }
  forward = !forward;
//  delay(500);
}

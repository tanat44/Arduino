#include "DriveServo.h"
#include "Const.h"

#define DISTANCE_TO_MM 0.59144// actual 197.82 mm, command distance = 117

DriveServo::DriveServo(){}

DriveServo::DriveServo(Servo *servo, int pin, int minStopValue, int maxStopValue, bool reverse, float speedConstant) 
: servo(servo), pin(pin), minStopValue(minStopValue), maxStopValue(maxStopValue), reverse(reverse), remainingMoveFrame(0), speedConstant(speedConstant) {
  Serial.print("Drive attached to ");
  Serial.println(pin, DEC);
  servo->attach(pin);
  setSpeed(0);
}

void DriveServo::setSpeed(int s){
  rotationSpeed = s;
  
  if (reverse)
    s *= -1;

  int value = (minStopValue + maxStopValue)/2;
  if (s > 0) {
    value = s + maxStopValue;
  } else if (s < 0) {
    value = minStopValue + s;
  } 

  servo->writeMicroseconds(value);
}

void DriveServo::move(int distanceMm, float linearSpeed){
  remainingMoveFrame = float(distanceMm) * DISTANCE_TO_MM / abs(linearSpeed) * CONTROL_FPS;
  rotationSpeed = linearSpeed * speedConstant;  
  setSpeed(rotationSpeed);
}

float DriveServo::calculateSpeed(int distance, int numFrame){
  return float(distance) * DISTANCE_TO_MM * CONTROL_FPS / numFrame;
}

void DriveServo::update(){
  if (remainingMoveFrame > 0){
    --remainingMoveFrame;
  } else {
    setSpeed(0);
  }
}

bool DriveServo::isReverse() {
  return reverse;
}

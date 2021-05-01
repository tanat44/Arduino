#include "SteerServo.h"

SteerServo::SteerServo(){}

SteerServo::SteerServo(Servo* servo, int pin, int centerValue) : servo(servo), pin(pin), centerValue(centerValue) {
  Serial.print("Steering attached to ");
  Serial.println(pin, DEC);
  servo->attach(pin);
  setPosition(0);
}

void SteerServo::setPosition(int pos){
  value = pos + centerValue;

  if (value == servo->read())
    return;

  servo->write(value);
}

int SteerServo::readPosition() {
  return value - centerValue;
}

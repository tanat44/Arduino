#include "Arduino.h"
#include "Servo.h"
#define DELAY 2

Servo::Servo(char _pin0, int _maxStep, bool _reverse){
  
  pin0 = _pin0;
  maxStep = _maxStep;
  reverse = _reverse;
  if (reverse) {
    currentStep = _maxStep;
  } else {
    currentStep = 0;
  }
  pinMode(_pin0, OUTPUT);
  pinMode(_pin0+1, OUTPUT);
  pinMode(_pin0+2, OUTPUT);
  pinMode(_pin0+3, OUTPUT);
}


Direction Servo::inverse(Direction d) {
  if (reverse) {
    if (d == Direction::Forward) {
      return Direction::Backward;
    } else {
      return Direction::Forward;
    }
    
  } else {
    return d;
  }
}

void Servo::driveSteps(Direction d, int steps) {
  d = inverse(d);

  //  if (currentStep > maxStep) {
//    currentStep = maxStep;
//    return;
//  } else if (currentStep < 0) {
//    currentStep = 0;
//    return;
//  }
  
  for (int i=0; i < steps; ++i) {
    driveOneStep(d);
    delay(DELAY);
  }
}


void Servo::driveOneStep(Direction d) {
 
  if (d == Forward) {
    ++currentStep;
  } else {
    --currentStep;
  }
  
  if ( currentStep%4 == 0) {
    digitalWrite(pin0, HIGH); 
    digitalWrite(pin0+1, HIGH); 
    digitalWrite(pin0+2, LOW); 
    digitalWrite(pin0+3, LOW); 
  } else if ( currentStep%4 == 1) {
    digitalWrite(pin0, LOW); 
    digitalWrite(pin0+1, HIGH); 
    digitalWrite(pin0+2, HIGH); 
    digitalWrite(pin0+3, LOW); 
  } else if ( currentStep%4 == 2) {
    digitalWrite(pin0, LOW); 
    digitalWrite(pin0+1, LOW); 
    digitalWrite(pin0+2, HIGH); 
    digitalWrite(pin0+3, HIGH); 
  } else if ( currentStep%4 == 3) {
    digitalWrite(pin0, HIGH); 
    digitalWrite(pin0+1, LOW); 
    digitalWrite(pin0+2, LOW); 
    digitalWrite(pin0+3, HIGH); 
  } 

}


void Servo::driveToEnd(Direction d) {

  d = inverse(d);
  for (int i=0; i < maxStep; ++i) {
    driveOneStep(d);
    delay(DELAY);
  }
}


void Servo::driveHome() {
  Serial.println("Driving Home");

  if (reverse) {
    currentStep = 0;
  } else {
    currentStep = maxStep;  
  }
  
  driveToEnd(Direction::Backward);
  if (reverse) {
    currentStep = maxStep;
  } else {
    currentStep = 0;
  }
}

void Servo::off() {
  digitalWrite(pin0, LOW); 
  digitalWrite(pin0+1, LOW); 
  digitalWrite(pin0+2, LOW); 
  digitalWrite(pin0+3, LOW); 
}

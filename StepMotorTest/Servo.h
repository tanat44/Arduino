#ifndef SERVO_H
#define SERVO_H

#include "Arduino.h"

enum Direction {
  Forward,
  Backward
};

class Servo
{
  public:
    Servo(char pin0, int maxStep, bool reverse);
    void driveSteps(Direction d, int steps);
    void driveToEnd(Direction d);
    void driveHome();
    void off();
    Direction inverse(Direction d);

  private:
    void driveOneStep(Direction d);
    
    int currentStep;
    char pin0;
    int maxStep;
    bool reverse;
};

#endif

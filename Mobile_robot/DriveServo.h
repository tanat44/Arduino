#ifndef DriveServo_h
#define DriveServo_h

#include "HardwareSerial.h"
#include "Servo.h"

#define WHEEL_RADIUS 32


class DriveServo {
public:
  DriveServo();
  DriveServo(Servo* servo, int pin, int minStopValue, int maxStopValue, bool reserve, float speedConstant = 1.0);
  void setSpeed(int s);
  void move(int distanceMm, float linearSpeed = 32.0);
  float calculateSpeed(int distance, int numFrame);
  void update();
  bool isReverse();

private:
  Servo *servo;
  unsigned char pin;
  int minStopValue;
  int maxStopValue;
  bool reverse;
  float speedConstant;

  int remainingMoveFrame;
  int rotationSpeed;
};

#endif

#ifndef DriveSteerModule_h
#define DriveSteerModule_h

#include "DriveServo.h"
#include "SteerServo.h"

class DriveSteerModule {
public:
  DriveSteerModule();
  DriveSteerModule(DriveServo* drive, SteerServo* steer);

  void steer(int angle, int steerSpeed);
  void drive(int distance, float speed);
  void update();
  bool isReady();


private:
  DriveServo *driveServo;
  SteerServo* steerServo;

  int currentAngle;
  int targetAngle;
  float steerSpeed;
  int remainingFrame;
  bool ready;
};


#endif;

#ifndef Robot_h
#define Robot_h

#include "DriveSteerModule.h"

enum SteerMode{
  Parallel,
  Spin,
  FrontSteer,
  BackSteer,
  LeftSteer,
  RightSteer,
};

class Robot{
  
public:
  Robot();
  Robot(DriveSteerModule *m1, DriveSteerModule *m2, DriveSteerModule *m3);
  void update();
  
  void steerParallel(int angle);
  void driveParallel(int distance, float speed);
  
  void steerSpin();
  void driveSpin(int distance, float speed);

private:
  DriveSteerModule *m1;
  DriveSteerModule *m2;
  DriveSteerModule *m3;
  SteerMode steerMode;
};

#endif

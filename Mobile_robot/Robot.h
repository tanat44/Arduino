#ifndef Robot_h
#define Robot_h

#define JOB_QUEUE_SIZE 5

#include "DriveSteerModule.h"
#include "Job.h"

class Robot{
  
public:
  Robot();
  Robot(DriveSteerModule *m1, DriveSteerModule *m2, DriveSteerModule *m3);
  void update();
  void runNextJob();
  void addJobQueue(Job::Type type, int param1, float param2);
  
  void steerParallel(int angle);
  void driveParallel(int distance, float speed);
  
  void steerSpin();
  void driveSpin(int distance, float speed);

private:
  DriveSteerModule *m1;
  DriveSteerModule *m2;
  DriveSteerModule *m3;
  Job::Type steerMode;

  uint8_t nextJob;
  uint8_t currentJob;  
  Job jobQueue[JOB_QUEUE_SIZE];
};

#endif

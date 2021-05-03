#include "Robot.h"
#include "Const.h"
#define DEFAULT_STEER_SPEED 4


Robot::Robot(): Robot(NULL, NULL, NULL) {}

Robot::Robot(DriveSteerModule *m1, DriveSteerModule *m2, DriveSteerModule *m3) 
: m1(m1), m2(m2), m3(m3), steerMode(Job::Type::None) {
  
}

void Robot::update() {
  m1->update();
  m2->update();
  m3->update();

  if (m1->isReady() && m2->isReady() && m3->isReady()){
    runNextJob();
  }
}

void Robot::runNextJob() {

  // There's no more job
  if (currentJob == nextJob)
    return;
  
  Job j = jobQueue[currentJob++];
  if (j.type == Job::Type::SteerParallel){
      steerParallel(j.param1);
  } else if (j.type == Job::Type::DriveParallel){
      driveParallel(j.param1, j.param2);
  } else if (j.type == Job::Type::SteerSpin){
      steerSpin();
  } else if (j.type == Job::Type::DriveSpin){
      driveSpin(j.param1, j.param2);
  }
   
  if (currentJob > JOB_QUEUE_SIZE-1) {
    currentJob = 0;
  }
}

void Robot::addJobQueue(Job::Type type, int param1, float param2) {
  jobQueue[nextJob++] = Job(type, param1, param2);
  if (nextJob > JOB_QUEUE_SIZE-1) {
    nextJob = 0;
  }
}


void Robot::steerParallel(int angle) {
  m1->steer(angle, DEFAULT_STEER_SPEED);
  m2->steer(angle, DEFAULT_STEER_SPEED);
  m3->steer(angle, DEFAULT_STEER_SPEED);
  steerMode = Job::Type::SteerParallel;
}

void Robot::driveParallel(int distance, float speed) {
  if (steerMode == Job::Type::SteerParallel) {
    m1->drive(distance, speed);
    m2->drive(distance, speed);
    m3->drive(distance, speed);
  } else {
    steerParallel(0);
    addJobQueue(Job::Type::DriveParallel, distance, speed);
  }
}

void Robot::steerSpin() {
  m1->steer(CENTER_SPIN_ANGLE, DEFAULT_STEER_SPEED);
  m2->steer(-CENTER_SPIN_ANGLE, DEFAULT_STEER_SPEED);
  m3->steer(CENTER_SPIN_ANGLE, DEFAULT_STEER_SPEED);
  steerMode = Job::Type::SteerSpin;
}

void Robot::driveSpin(int distance, float speed) {
  if (steerMode == Job::Type::SteerSpin) {
    m1->drive(distance, speed);
    m2->drive(distance, -speed);
    m3->drive(distance, -speed);
  } else {
    steerSpin();
    addJobQueue(Job::Type::DriveSpin, distance, speed);
  }
}

#include "Robot.h"
#include "Const.h"
#define DEFAULT_STEER_SPEED 4

Robot::Robot(){}

Robot::Robot(DriveSteerModule *m1, DriveSteerModule *m2, DriveSteerModule *m3) : m1(m1), m2(m2), m3(m3), steerMode(SteerMode::Parallel) {
  
}

void Robot::update() {
  m1->update();
  m2->update();
  m3->update();
}


void Robot::steerParallel(int angle) {
  m1->steer(angle, DEFAULT_STEER_SPEED);
  m2->steer(angle, DEFAULT_STEER_SPEED);
  m3->steer(angle, DEFAULT_STEER_SPEED);
}

void Robot::driveParallel(int distance, float speed) {
  m1->drive(distance, speed);
  m2->drive(distance, speed);
  m3->drive(distance, speed);
}

void Robot::steerSpin() {
  m1->steer(CENTER_SPIN_ANGLE, DEFAULT_STEER_SPEED);
  m2->steer(-CENTER_SPIN_ANGLE, DEFAULT_STEER_SPEED);
  m3->steer(CENTER_SPIN_ANGLE, DEFAULT_STEER_SPEED);
}

void Robot::driveSpin(int distance, float speed) {
  m1->drive(distance, speed);
  m2->drive(distance, -speed);
  m3->drive(distance, -speed);
}

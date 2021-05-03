#include "DriveSteerModule.h"
#define STEER_SPEED_CONSTANT 0.5
#define STEER_ANGLE_TO_DRIVE_DISTANCE 0.4

DriveSteerModule::DriveSteerModule(){}

DriveSteerModule::DriveSteerModule(DriveServo* driveServo, SteerServo* steerServo): driveServo(driveServo), steerServo(steerServo), currentAngle(0), ready(true){
  
}

void DriveSteerModule::drive(int distance, float speed){
  driveServo->move(distance, speed);
}

void DriveSteerModule::steer(int angle, int speed){
  
  if (angle < -60) {
    angle = -60;
  } else if (angle > 60) {
    angle = 60;
  }

  int direction = angle > steerServo->readPosition() ? 1 : -1;
  
  targetAngle = angle;
  steerSpeed = float(speed * direction) * STEER_SPEED_CONSTANT;
  remainingFrame = float(abs(angle - steerServo->readPosition())) / abs(steerSpeed);

  if (remainingFrame > 0) {
    ready = false;
    int driveDistance = float(abs(angle - steerServo->readPosition())) * STEER_ANGLE_TO_DRIVE_DISTANCE;
    float driveSpeed = driveServo->calculateSpeed(driveDistance, remainingFrame);
    int servoReverse = driveServo->isReverse() ? -1 : 1;
    driveServo->move(driveDistance, driveSpeed * servoReverse * direction);  
  }  
}


void DriveSteerModule::update(){
  driveServo->update();  
  
  if (remainingFrame > 0){
    currentAngle += steerSpeed;
    steerServo->setPosition(currentAngle);
    --remainingFrame;
  } else {
    steerServo->setPosition(targetAngle);
    ready = true;
  }
}


bool DriveSteerModule::isReady(){
  return ready;
}

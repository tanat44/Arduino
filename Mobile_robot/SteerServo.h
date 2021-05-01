#ifndef SteerServo_h
#define SteerServo_h

#include "HardwareSerial.h"
#include "Servo.h"

class SteerServo {
public:
  SteerServo();
  SteerServo(Servo* servo, int pin, int centerValue);
  void setPosition(int pos);
  int readPosition();

private:
  Servo *servo;
  unsigned char pin;
  int value;
  unsigned char centerValue;
  
};

#endif

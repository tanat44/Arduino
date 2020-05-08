#include <Timer.h>
#include <Servo.h>
#include "Timer.h"
#define minPitch 150
#define maxPitch 50
#define maxSpeed 100
 
Timer t;
Servo feedServo;
Servo pitchServo; 
Servo esc;

const int analogInPinY = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPinX = A1; 
const int switchPin = 13;

const unsigned char posOff = 30;
const unsigned char posOn = 100;
bool motorOn = 0;
int pitch = 20;
int motorSpeed = 20;
int frame = 0;

void setup() {
  Serial.begin(9600);
  feedServo.attach(9);  // attaches the servo on pin 9 to the servo object
  pitchServo.attach(8);
  esc.attach(10);
  esc.write(20);
  delay(3000);
  pinMode(LED_BUILTIN, OUTPUT);
  t.every(100, feedControl);
}


void setPitch(unsigned char p){
  int o = map(p, 0, 100, minPitch, maxPitch);
  pitchServo.write(o);
}

void setSpeed(unsigned char s){
  int o = map(s, 0, 100, 20, maxSpeed);
  esc.write(o);
}

void loop() {
  int analogX = analogRead(analogInPinX); //left = high, right = row
  int analogY = analogRead(analogInPinY);
//  Serial.print("X = ");
//  Serial.print(analogX);
//  Serial.print(" ,Y = ");
//  Serial.println(analogY);

  if (analogX < 100) {
    motorOn = !motorOn;
    delay(300);
  }

  if (analogX > 800) {
    if (pitch < 100) {
      pitch += 10;
    } else {
      for (; pitch >0; pitch -= 2){
        setPitch(pitch);
        delay(20);
      }
    }
    delay(400);
  }
  if (analogY > 800){
    motorSpeed -= 10;
    if (motorSpeed < 0){
      motorSpeed = 0;
    }
    delay(200);
  }

  if (analogY < 100){
    motorSpeed += 10;
    if (motorSpeed > 100){
      motorSpeed = 100;
    }
    delay(200);
  }

  if (motorOn) {
    setSpeed(motorSpeed);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    setSpeed(0);
    digitalWrite(LED_BUILTIN, LOW);
  }
  setPitch(pitch);
  t.update();

  delay(2);
  
}

void feedControl() {
//  Serial.println(frame);
  if (motorOn) {
    if (frame < 20) {
      feedServo.write(posOff);
    } else if (frame < 21) {
      feedServo.write(posOn);
    } else if (frame < 26) {
      feedServo.write(posOff);
    } else if (frame < 34) {
      feedServo.write(posOn);
    } else {
      frame = 0;
    }
    frame += 1;
  }  
}


void testServoSerial(Servo s){
  int sensorValue = analogRead(analogInPinY);
  int outputValue = map(sensorValue, 0, 1023, 0, 180);
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);
  s.write(outputValue);
}

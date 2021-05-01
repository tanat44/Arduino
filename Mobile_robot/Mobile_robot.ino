#include "Servo.h"
#include "SteerServo.h"
#include "DriveServo.h"
#include "DriveSteerModule.h"
#include "Robot.h"


Servo servo1, servo2, servo3, servo4, servo5, servo6;
SteerServo s1, s2, s3;
DriveServo d1, d2, d3;
DriveSteerModule m1, m2, m3;
Robot robot;
int angle[] = {0,0,0};
int speed[] = {0,0,0};


void setupTimer0() {
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 250; // = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22);      // 64 prescaler  
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}

// FRAME TIMER
#define TIMER_COUNT_PER_FRAME 25
uint16_t frameTimerCount = 0;
ISR(TIMER2_COMPA_vect){
  if (++frameTimerCount > TIMER_COUNT_PER_FRAME){
    handleFrame();
    frameTimerCount = 0;
  }
}

void handleFrame() {
  robot.update();
//  d1.update();
//  d2.update();
//  d3.update();
//  m1.update();
//  m2.update();
//  m3.update();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  s1 = SteerServo(&servo1, 2, 116);
  s2 = SteerServo(&servo2, 4, 83);
  s3 = SteerServo(&servo3, 6, 90);
  d1 = DriveServo(&servo4, 3, 1430, 1470, false, 0.92);
  d2 = DriveServo(&servo5, 5, 1520, 1540, true, 1.1);
  d3 = DriveServo(&servo6, 7, 1490, 1520, true, 0.95);
  m1 = DriveSteerModule(&d1, &s1);
  m2 = DriveSteerModule(&d2, &s2);
  m3 = DriveSteerModule(&d3, &s3);
  robot = Robot(&m1, &m2, &m3);

  setupTimer0();
  Serial.println("=== Ready ===");
}

void printState() {
  Serial.print("ANGLE ");
  Serial.print(angle[0], DEC);
  Serial.print(" ");
  Serial.print(angle[1], DEC);
  Serial.print(" ");
  Serial.print(angle[2], DEC);
  Serial.print(" SPEED ");
  Serial.print(speed[0], DEC);
  Serial.print(" ");
  Serial.print(speed[1], DEC);
  Serial.print(" ");
  Serial.println(speed[2], DEC);
}


void loop() {
  while (Serial.available() > 0) {
    String terminalText = Serial.readStringUntil('\n');
    int len = terminalText.length();

    // STEERING COMMAND
    if (terminalText[0] == 'a'){
      int number = terminalText.substring(2).toInt();
      // CRAB WALK
      if (terminalText[1] == 'a') {
        angle[0] = number;
        angle[1] = number;
        angle[2] = number;
        
      // FRONT STEER
      } else if (terminalText[1] == 'b') {
        angle[0] = number;
        angle[1] = number;
        angle[2] = 0;
        
      // BACK STEER
      } else if (terminalText[1] == 'c') {
        angle[0] = 0;
        angle[1] = 0;
        angle[2] = number;
      }

    // MANUAL CONTROL
    } else if (terminalText[0] == 'm') {
      int indexes[5];
      int count = 0;
      for (int i = 1; i<len; ++i) {
        if (terminalText[i] == ' '){
          indexes[count++] = i;
        }
      }
      angle[0] = terminalText.substring(1, indexes[0]).toInt();
      angle[1] = terminalText.substring(indexes[0] + 1, indexes[1]).toInt();
      angle[2] = terminalText.substring(indexes[1] + 1, indexes[2]).toInt();
      speed[0] = terminalText.substring(indexes[2] + 1, indexes[3]).toInt();
      speed[1] = terminalText.substring(indexes[3] + 1, indexes[4]).toInt();
      speed[2] = terminalText.substring(indexes[4] + 1).toInt();

      
    // STEER PARALLEL
    } else if (terminalText.substring(0,2) == "sp") {
      int angle = terminalText.substring(2).toInt();
      robot.steerParallel(angle);

    // STEER PARALLEL
    } else if (terminalText.substring(0,2) == "ss") {
      robot.steerSpin();

    // DRIVE PARALLEL
    } else if (terminalText.substring(0,2) == "dp") {
      int distance = 10;
      int direction = 1;
      if(len > 2) {
        distance = terminalText.substring(2).toInt();
        if (distance < 0){
          distance = abs(distance);
          direction = -1;
        }
      }
      robot.driveParallel(distance, 40*direction);

    // DRIVE SPIN
    } else if (terminalText.substring(0,2) == "ds") {
      int distance = 10;
      int direction = 1;
      if(len > 2) {
        distance = terminalText.substring(2).toInt();
        if (distance < 0){
          distance = abs(distance);
          direction = -1;
        }
      }
      robot.driveSpin(distance, 40*direction);

    // DRIVE
    } else if (terminalText[0] == 'd') {
      int distance = 10;
      int direction = 1;
      if(len > 1) {
        distance = terminalText.substring(1).toInt();
        if (distance < 0){
          distance = abs(distance);
          direction = -1;
        }
      }
      m1.drive(distance, 40*direction);
      m2.drive(distance, 40*direction);
      m3.drive(distance, 40*direction);

      
    // STEER
    } else if (terminalText[0] == 'q') {
      int steerSpeed = 4;
      int angle = 0;
      if(len > 1) {
        angle = terminalText.substring(1).toInt();
      }
      m1.steer(angle, steerSpeed);
      m2.steer(angle, steerSpeed);
      m3.steer(angle, steerSpeed);

    } else if (terminalText[0] == '+' || terminalText[0] == '-') {      
      int value = terminalText.substring(1).toInt();
      if (terminalText[0] == '-') {
        value *= -1;
      }
      speed[0] += value;
      speed[1] += value;
      speed[2] += value;
      printState();
      
    // RESET COMMAND
    } else if (terminalText[0] == '0') {
      angle[0] = 0;
      angle[1] = 0;
      angle[2] = 0;
      speed[0] = 0;
      speed[1] = 0;
      speed[2] = 0;
    }
      
  }

}

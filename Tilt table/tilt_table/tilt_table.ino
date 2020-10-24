#include <Timer.h>
#include <Servo.h>

// tunable parameters
#define MIN_PITCH   62        // reversing these values will reverse the servo direction
#define MAX_PITCH   76
#define SERVO_STEP  100 

#define UP_START_STEP   50      
#define UP_SPEED        35
#define UP_END_STEP     5
#define UP_END_SPEED    20
#define DOWN_SPEED      80        // increase this value to reduce the speed

#define MAX_LIFT_DURATION 300  // in seconds

// program flow control
#define TEST_MODE false

// hardware pin
#define UP_BTN 2
#define DOWN_BTN 3
 
Timer t;
Servo feedServo;
Servo pitchServo; 
Servo esc;

enum Direction { up, down };

int pitch = 0;
Direction testDirection = Direction::up;

// for slow in and out movement
int delayedServoOut = -1;
int slowEvent;

int stopSecond = 0;
int stopSecondEvent = -1;
bool returningHome = false;

void writeSlowServo(){
  if (delayedServoOut == -1){
    return;
  }
  Serial.println("slow servo ");
  Serial.println(delayedServoOut, DEC);
  pitchServo.write(delayedServoOut);

  // clean up slow flag
  delayedServoOut = -1;
  t.stop(slowEvent);
}

void resetStopSecondEvent() {
    t.stop(stopSecondEvent);
    stopSecondEvent = -1;
    stopSecond = 0;
}

void tickStopSecond() {
  if (stopSecond > MAX_LIFT_DURATION){
    returningHome = true;
    resetStopSecondEvent();
  } else {
    ++stopSecond;
    Serial.print("Return home timer elapsed for ");
    Serial.print(stopSecond, DEC);
    Serial.println("s");
  }
}


// setPitch receive value 0 to SERVO_RESOLUTION
void setPitch(Direction d){
  
  // time for SLOW_STEP is running
  if (delayedServoOut > -1) {
    return;
  }
  
  if (d == Direction::up){
    if (pitch >= SERVO_STEP) {
      pitch = SERVO_STEP;
      if (TEST_MODE){
        testDirection = Direction::down;
      }
      return;
    } 

    if (pitch < UP_START_STEP){
      pitch += 15;
    } else {
      ++pitch;
    }
  } else if (d == Direction::down){
    if (pitch <= 0) {
      pitch = 0;
      if (TEST_MODE){
        testDirection = Direction::up;
      }
      if (returningHome) {
        returningHome = false;
        
      }
      return;
    } 
    --pitch;
  } else {
    Serial.println("ERROR setPitch with undefined direction");
    return;
  }

  if (stopSecondEvent != -1){
    resetStopSecondEvent();
  }
  stopSecondEvent = t.every(1000, tickStopSecond);
  
  // remapping pitch to defined range of motion
  int o = map(pitch, 0, SERVO_STEP, MIN_PITCH, MAX_PITCH);
  Serial.print("Pitch = ");
  Serial.print(pitch, DEC);
  Serial.print(" Servo = ");  
  Serial.print(o, DEC);
  Serial.println("");

  delayedServoOut = o;
  if (d == Direction::up){
    if (pitch < SERVO_STEP - UP_END_STEP) {
      slowEvent = t.after(UP_SPEED, writeSlowServo);
    } else if (pitch >= SERVO_STEP - UP_END_STEP) {
      slowEvent = t.after(UP_END_SPEED, writeSlowServo);
    }
  } else {
    // coming down
    slowEvent = t.after(DOWN_SPEED, writeSlowServo);
  }
  
}

void setup() {
  Serial.begin(9600);
  pitchServo.attach(9);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(UP_BTN, INPUT);
  pinMode(DOWN_BTN, INPUT);
  Serial.println("Initilize");
  setPitch(0);
}

void loop() {
  t.update();

  if (TEST_MODE) {
    setPitch(testDirection);
    return;
  }

  if (returningHome) {
    Serial.println("Returning home");
    setPitch(Direction::down);
    return;
  }
  
  if (!digitalRead(UP_BTN))
  {
    Serial.println("UP pressed");
    setPitch(Direction::up);
  }
  else if (!digitalRead(DOWN_BTN))
  {
    Serial.println("DOWN pressed");
    setPitch(Direction::down);
  } 
}

#include <Timer.h>
#include <Servo.h>

#define MIN_PITCH   62   // reversing these values will reverse the servo direction
#define MAX_PITCH   76
#define SERVO_STEP  100    
#define SLOW_STEP   5    // servo will slow at begin and end of motion,  this value should be below SERVO_STEP / 2
#define SLOW_SPEED  20    // increase this value to reduce the speed
#define NORMAL_SPEED  35

#define UP_BTN 2
#define DOWN_BTN 3
#define TEST_MODE false

 
Timer t;
Servo feedServo;
Servo pitchServo; 
Servo esc;

enum Direction { up, down };
const int analogInPinY = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPinX = A1; 
const int switchPin = 13;
int pitch = 0;
Direction testDirection = Direction::up;

// for slow in and out movement
int delayedServoOut = -1;
int slowEvent;

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
    ++pitch;
  } else if (d == Direction::down){
    if (pitch <= 0) {
      pitch = 0;
      if (TEST_MODE){
        testDirection = Direction::up;
      }
      return;
    } 
    --pitch;
  } else {
    Serial.println("ERROR setPitch with undefined direction");
  }

  // remapping pitch to defined range of motion
  int o = map(pitch, 0, SERVO_STEP, MIN_PITCH, MAX_PITCH);
  Serial.print(pitch, DEC);
  Serial.print(" ");  
  Serial.print(o, DEC);
  Serial.println("");

  delayedServoOut = o;
  if (pitch < SLOW_STEP || pitch > SERVO_STEP - SLOW_STEP ) {
    // write servo in slow speed during SLOW_STEP
    slowEvent = t.after(SLOW_SPEED, writeSlowServo);
  } else {
    // write servo in normal speed
    slowEvent = t.after(NORMAL_SPEED, writeSlowServo);
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


// DIGITAL READ
//  int analogX = analogRead(analogInPinX); //left = high, right = row
//  int analogY = analogRead(analogInPinY);
//  Serial.print("X = ");
//  Serial.print(analogX);
//  Serial.print(" ,Y = ");
//  Serial.println(analogY);

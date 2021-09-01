#include <ESP8266WiFi.h>
//#define PRINT_ENCODER_POSITION

// TESTED ON 12v 775 MOTOR
#define KP 6.0
#define KD 11.0

uint8_t ENCODER1_PIN = D2;
uint8_t ENCODER2_PIN = D1;
uint8_t MOTOR1_PIN = D5;
uint8_t MOTOR2_PIN = D6;
uint8_t MOTOREN_PIN = D7;

bool forwardDirection = true;
int position = 0;
int speed = 10;

void setup() {
 Serial.begin(9600);
 Serial.println("Starting");

 // ENCODER
 pinMode(ENCODER1_PIN, INPUT);
 pinMode(ENCODER2_PIN, INPUT);
 attachInterrupt(digitalPinToInterrupt(ENCODER1_PIN), Encoder1Handle, RISING);
// attachInterrupt(digitalPinToInterrupt(ENCODER2_PIN), Encoder2Handle, RISING);

 // MOTOR
 pinMode(MOTOR1_PIN, OUTPUT);
 pinMode(MOTOR2_PIN, OUTPUT);
 pinMode(MOTOREN_PIN, OUTPUT);

 setMotorSpeed(speed);

}

void setMotorSpeed(int s){
  int newSpeed = map(abs(s), 0, 1024, 150, 1024);
  if (s > 0) {
    digitalWrite(MOTOR1_PIN, 1);
    digitalWrite(MOTOR2_PIN, 0);
    analogWrite(MOTOREN_PIN, newSpeed);
  } else if (s == 0){
    digitalWrite(MOTOR1_PIN, 0);
    digitalWrite(MOTOR2_PIN, 0);
  } else {
    digitalWrite(MOTOR1_PIN, 0);
    digitalWrite(MOTOR2_PIN, 1);
    analogWrite(MOTOREN_PIN, newSpeed);
  }
  
}

float kp = KP;
float kd = KD;
int lastError = 0;
int setPoint = 0;

void positionHolding() {
  int error = setPoint - position;
  
  int proportion = (int)(kp*error);
  int diff = (int) (kd*(error - lastError));
  int newPoint = proportion + diff;

  Serial.print("kp=");
  Serial.print(kp, DEC);
  Serial.print("\t kd=");
  Serial.print(kd, DEC);
  Serial.print("\t error=");
  Serial.print(error, DEC);
  Serial.print("\t");
  Serial.print(proportion, DEC);
  Serial.print("\t");
  Serial.print(diff, DEC);
  Serial.print("\t");
  Serial.print(newPoint, DEC);
  Serial.println();
  
  setMotorSpeed( newPoint );
  lastError = error;
}

void cyclePosition() {
  uint8_t s = 10;
  if (position > 10) {
    setMotorSpeed(-s);
  } else if (position < -10) {
    setMotorSpeed(s);
  }
}

void loop() {
  if(Serial.available() > 0)  {
    String data= Serial.readStringUntil('\n'); // can be -1 if read error
    if (data == "7") { 
      kp += 1;
    } else if (data == "4"){
      kp -= 1;
    } else if (data == "8"){
      kd += 1;
    } else if (data == "5"){
      kd -= 1;
    } else if (data == "0"){
      position = 0;
      kp = KP;
      kd = KD;
      lastError = 0;
      setMotorSpeed(0);
      delay(2000);
    }
 }
  positionHolding();
}

void printResult() {
  Serial.print("POS= ");
  Serial.println(position, DEC);
}

ICACHE_RAM_ATTR void Encoder1Handle(){
  if (digitalRead(ENCODER1_PIN)) {
    if (digitalRead(ENCODER2_PIN)) {
      ++position;
    } else {
      --position;
    }
  } else {
    if (digitalRead(ENCODER2_PIN)) {
      --position;
    } else {
      ++position;
    }
  }
  #ifdef PRINT_ENCODER_POSITION
  printResult();
  #endif
}

ICACHE_RAM_ATTR void Encoder2Handle(){
  if (digitalRead(ENCODER2_PIN)) {
    if (digitalRead(ENCODER1_PIN)) {
      --position;
    } else {
      ++position;
    }
  } else {
    if (digitalRead(ENCODER1_PIN)) {
      ++position;
    } else {
      --position;
    }
  }
  #ifdef PRINT_ENCODER_POSITION
  printResult();
  #endif

}

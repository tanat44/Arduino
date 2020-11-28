int pin1 = 2;
int pin2 = 3;
int pin3 = 4;
int pin4 = 5;

void setup() {

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
}

void driveOnePhase( int x) {
  if (x == 0) {
    digitalWrite(pin1, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, LOW); 
  } else if (x == 1) {
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin1, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, LOW); 
  } else if (x == 2) {
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin1, LOW); 
    digitalWrite(pin4, LOW); 
  } else if (x == 3) {
    digitalWrite(pin4, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin1, LOW); 
  }
}

void driveTwoPhase( int x) {
  if (x == 0) {
    digitalWrite(pin1, HIGH); 
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, LOW); 
  } else if (x == 1) {
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin1, LOW); 
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin4, LOW); 
  } else if (x == 2) {
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin1, LOW); 
    digitalWrite(pin4, HIGH); 
  } else if (x == 3) {
    digitalWrite(pin4, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin1, HIGH); 
  }
}

void driveHalfStep( int x) {
  if (x == 0) {
    digitalWrite(pin1, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, LOW); 
  } else if (x == 1) {
    digitalWrite(pin1, HIGH); 
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, LOW);
  } else if (x == 2) {
    digitalWrite(pin1, LOW); 
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, LOW);
  } else if (x == 3) {
    digitalWrite(pin1, LOW); 
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin4, LOW);
  } else if (x == 4) {
    digitalWrite(pin1, LOW); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin4, LOW);
  } else if (x == 5) {
    digitalWrite(pin1, LOW); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin4, HIGH);
  } else if (x == 6) {
    digitalWrite(pin1, LOW); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, HIGH);
  } else if (x == 7) {
    digitalWrite(pin1, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, HIGH);
  }
}

static int currentStep = 0;
  
void loop() {

  driveTwoPhase(currentStep++);
  if (currentStep == 8){
    currentStep = 0;
  }
  delay(3);                
}

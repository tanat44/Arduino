int pin1 = 0;
int pin2 = 1;
int pin3 = 2;
int pin4 = 3;

enum Direction {
  Forward,
  Backward
};

static int currentStep = 0;

void driveTwoPhase(Direction x) {
  if (x == Forward) {
    if (currentStep > 3) {
      currentStep = 0;
    }
  } else if (x == Backward) {
    if (currentStep < 0) {
      currentStep = 3;
    }
  }
  
  if (currentStep == 0) {
    digitalWrite(pin1, HIGH); 
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, LOW); 
  } else if (currentStep == 1) {
    digitalWrite(pin1, LOW); 
    digitalWrite(pin2, HIGH); 
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin4, LOW); 
  } else if (currentStep == 2) {
    digitalWrite(pin1, LOW); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, HIGH); 
    digitalWrite(pin4, HIGH); 
  } else if (currentStep == 3) {
    digitalWrite(pin1, HIGH); 
    digitalWrite(pin2, LOW); 
    digitalWrite(pin3, LOW); 
    digitalWrite(pin4, HIGH); 
  } 

  if (x == Forward) {
    ++currentStep;
  } else if (x == Backward) {
    --currentStep;
  }
}

void moveToEnd(Direction d) {
  for (int i=0; i<1250; ++i) {
    driveTwoPhase(d);
    delay(2);
  }
}

void setup() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  moveToEnd(Backward);
}

static bool run = true;
  
void loop() {
  if (run) {
    moveToEnd(Forward);
  } else {
    moveToEnd(Backward);
  }
  run = !run;
  delay(1000);
}

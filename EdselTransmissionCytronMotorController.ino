/*******************************************************************************
 * PIN CONFIGURATION                                                           *
 *******************************************************************************/

const int parkPin = 6;
const int reversePin = 7;
const int neutralPin = 8;
const int drivePin = 9;
const int lowPin = 10;

// PWM is connected to pin 11.
const int pinPwm = 11;

// DIR is connected to pin a digital pin 13.
const int pinDir = 13;

// Analog pins
const int potPos = 5;

/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES                                                     *
 *******************************************************************************/

// precision is about 3 units
int limits[2] = {40,870};	// set limitations {min,max}

int parkPos = 680; // 680
int reversePos = 540;
int neutralPos = 400; //240-380 = 140
int drivePos = 260; // 590 is max for drive
int lowPos = 100;  //980 is max

int parkState = 0; // when 1 then the park button is pressed
int reverseState = 0;
int neutralState = 0;
int driveState = 0;
int lowState = 0;

int dir = 1;
int inward = -1;
int outward = 1;

int leeway = 3;

boolean targetHit = false;
int lastStatePos = 0;

void setup() { 
  Serial.begin(9600);
        
  pinMode(parkPin, INPUT);
  pinMode(reversePin, INPUT);
  pinMode(neutralPin, INPUT);
  pinMode(drivePin, INPUT);
  pinMode(lowPin, INPUT);
  
  // Initialize the PWM and DIR pins as digital outputs.
  pinMode(pinPwm, OUTPUT);
  pinMode(pinDir, OUTPUT);
  
  pinMode(potPos, INPUT);
}

void moveMotor(int targetPos, int currentPos, boolean stopMotion) {
  if(stopMotion){
    dir = 0;
  } else if(
    currentPos == targetPos || 
    currentPos == targetPos + leeway || 
    currentPos == targetPos - leeway) {
    Serial.print(" - targetHit - ");
    dir = 0;
    targetHit = true;
  } else if(currentPos < targetPos || currentPos < limits[0]){
    dir = outward; 
  } else if(currentPos > targetPos || currentPos > limits[1]){
    dir = inward;
  }

  Serial.print(" - ");  
  Serial.print(dir);
  Serial.print(" - ");
  Serial.print(currentPos);
  Serial.print(':');
  Serial.print(targetPos);
  
  if(dir == 0){
    // no motion
    analogWrite(pinPwm, 0);
    digitalWrite(pinDir, LOW);
  } else if(dir == outward) {
    // outward
    Serial.print(" - outward");
    analogWrite(pinPwm, 255); 
    digitalWrite(pinDir, LOW);
  } else if(dir == inward) {
    // inward
    Serial.print(" - inward");
    analogWrite(pinPwm, 255);
    digitalWrite(pinDir, HIGH);
  }
}

void checkBeforeMove(int positionTarget, int potPosition, boolean stopMotion) {
  if(lastStatePos != positionTarget){
    targetHit = false; 
  }
  lastStatePos = positionTarget;
  if(!targetHit){
    moveMotor(positionTarget, potPosition, false);
  } else {
    Serial.print(" - no move - ");
    Serial.print(potPosition);
    Serial.print(":");
    Serial.print(positionTarget);
  }
}

void loop() 
{ 
  int potPosition =  analogRead(potPos);
  Serial.print(potPosition);
  parkState = digitalRead(parkPin);
  reverseState = digitalRead(reversePin);
  neutralState = digitalRead(neutralPin);
  driveState = digitalRead(drivePin);
  lowState = digitalRead(lowPin);
  
  if(parkState == 1){
    Serial.print("P");
    checkBeforeMove(parkPos, potPosition, false);
  } else if(reverseState == 1){
    Serial.print("R");
    checkBeforeMove(reversePos, potPosition, false);
  } else if(neutralState == 1){
    Serial.print("N");
    checkBeforeMove(neutralPos, potPosition, false);
  } else if(driveState == 1){
    Serial.print("D");
    checkBeforeMove(drivePos, potPosition, false);
  } else if(lowState == 1){
    Serial.print("L");
    checkBeforeMove(lowPos, potPosition, false);
  } else {
    // stop motor
    moveMotor(limits[0], potPosition, true);
  }
  
  Serial.println();
  
  delay(20);
} 

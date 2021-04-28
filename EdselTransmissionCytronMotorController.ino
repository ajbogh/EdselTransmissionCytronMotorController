/**
 * License: Modified GPL v3 - https://www.gnu.org/licenses/gpl-3.0.en.html
 * Copyright Allan Bogh (ajbogh@allanbogh.com)
 */

#include <limits.h>

/*******************************************************************************
   PIN CONFIGURATION
 *******************************************************************************/

//const int servoPin = 4
const int parkPin = 6;
const int reversePin = 7;
const int neutralPin = 8;
const int drivePin = 9;
const int lowPin = 10;

// const int parkPin = 8;
// const int reversePin = 9;
// const int neutralPin = 10;
// const int drivePin = 11;
// const int lowPin = 12;

// PWM is connected to pin 11.
const int pinPwm = 11;

// DIR is connected to pin a digital pin 13.
const int pinDir = 13;

// Analog pins
const int potPos = 5;
const int inhibitorPin = A0;
volatile int inhibitorValue = INT_MAX;

/*******************************************************************************
   PRIVATE GLOBAL VARIABLES
 *******************************************************************************/

// precision is about 3 units
int limits[2] = {4, 972};	// set limitations {min,max}

int parkPos = 972;
int reversePos = 825;
int neutralPos = 690;
int drivePos = 575;
int lowPos = 425;

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

/* Inhibitor notes:
    15-19 = Parked and stopped, no motion.
    slow reverse = 15
    faster than 5mph > 1000
    conclusion: > 100 = driving, don't shift to P, R, or N
*/
int inhibitorLimit = 100;

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

  pinMode(inhibitorPin, INPUT_PULLUP);
}

void moveMotor(int targetPos, int currentPos, boolean stopMotion) {
  if (stopMotion) {
    dir = 0;
  } else if (
    currentPos == targetPos ||
    currentPos == targetPos + leeway ||
    currentPos == targetPos - leeway) {
    Serial.print(" - targetHit - ");
    dir = 0;
    targetHit = true;
  } else if (currentPos < targetPos || currentPos < limits[0]) {
    dir = outward;
  } else if (currentPos > targetPos || currentPos > limits[1]) {
    dir = inward;
  }

  Serial.print(" - ");
  Serial.print(dir);
  Serial.print(" - ");
  Serial.print(currentPos);
  Serial.print(':');
  Serial.print(targetPos);

  if (dir == 0) {
    // no motion
    analogWrite(pinPwm, 0);
    digitalWrite(pinDir, LOW);
  } else if (dir == outward) {
    // outward
    Serial.print(" - outward");

    if (targetPos - currentPos > 50) {
      analogWrite(pinPwm, 255);
    } else {
      analogWrite(pinPwm, 128);
    }

    digitalWrite(pinDir, LOW);
  } else if (dir == inward) {
    // inward
    Serial.print(" - inward");

    if (currentPos - targetPos > 50) {
      analogWrite(pinPwm, 255);
    } else {
      analogWrite(pinPwm, 128);
    }

    digitalWrite(pinDir, HIGH);
  }
}

void checkBeforeMove(int positionTarget, int potPosition, boolean stopMotion) {
  if (lastStatePos != positionTarget) {
    targetHit = false;
  }
  lastStatePos = positionTarget;
  if (!targetHit) {
    moveMotor(positionTarget, potPosition, false);
  } else {
    Serial.print(" - no move - ");
    Serial.print(potPosition);
    Serial.print(":");
    Serial.print(positionTarget);
  }
}

boolean isInhibited() {
  return false; // TODO: remove
  if (inhibitorValue > inhibitorLimit) {
    return true;
  }
  return false;
}

void loop()
{
  inhibitorValue = analogRead(inhibitorPin);
  int potPosition =  analogRead(potPos);
  Serial.print(potPosition);
  parkState = digitalRead(parkPin);
  reverseState = digitalRead(reversePin);
  neutralState = digitalRead(neutralPin);
  driveState = digitalRead(drivePin);
  lowState = digitalRead(lowPin);

  Serial.print("==");
  Serial.print(parkState);
  Serial.print(reverseState);
  Serial.print(neutralState);
  Serial.print(driveState);
  Serial.print(lowState);
  Serial.print("==");

  if (parkState == HIGH && !isInhibited()) {
    Serial.print("P");
    checkBeforeMove(parkPos, potPosition, false);
  } else if (reverseState == HIGH && !isInhibited()) {
    Serial.print("R");
    checkBeforeMove(reversePos, potPosition, false);
  } else if (neutralState == HIGH && !isInhibited()) {
    Serial.print("N");
    checkBeforeMove(neutralPos, potPosition, false);
  } else if (driveState == HIGH) {
    Serial.print("D");
    checkBeforeMove(drivePos, potPosition, false);
  } else if (lowState == HIGH) {
    Serial.print("L");
    checkBeforeMove(lowPos, potPosition, false);
  } else {
    // stop motor
    moveMotor(limits[0], potPosition, true);
  }

  Serial.print(" i:");
  Serial.print(inhibitorValue);

  Serial.println();

  delay(20);
}

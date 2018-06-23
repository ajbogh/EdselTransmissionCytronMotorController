//#include <Servo.h> 

//SoftwareServo myservo;  // create servo object to control a servo 
//Servo myservo;

/*******************************************************************************
 * PIN CONFIGURATION                                                           *
 *******************************************************************************/

//const int servoPin = 4
const int parkPin = 8;
const int reversePin = 9;
const int neutralPin = 10;
const int drivePin = 11;
const int lowPin = 12;

// PWM is connected to pin 3.
const int pinPwm = 3;

// DIR is connected to pin a digital pin 2.
const int pinDir = 2;

// Analog pins
const int potPos = 5;

// precision is about 3 units
int limits[2] = {60,980};	// set limitations (min/max: 0->180)

int parkPos = 60; // 60-80 = 20
int reversePos = 165; // 100-230 = 130 // -130 == 1/2"
int neutralPos = 310; //240-380 = 140;
int drivePos = 465; //590;
int lowPos = 750;  //980; //460;

////int limits[2] = {1000,2000};    // set limitations (min/max: 1000->2000)
int parkState = 0; // when 1 then the park button is pressed
int reverseState = 0;
int neutralState = 0;
int driveState = 0;
int lowState = 0;

/*******************************************************************************
 * PRIVATE GLOBAL VARIABLES                                                     *
 *******************************************************************************/

// Speed of the motor.
static int iSpeed = 0;

// Acceleration of the motor.
static int iAcc = 1;

int dir = 1;
int currPos = 0;
int reverseCount = 0;

boolean targetHit = false;
int lastStatePos = 0;

void setup() 
{ 
  Serial.begin(9600);

        //myservo.attach(servoPin, limits[0], limits[1]);
        
        // set to neutral on startup
        // the button states will override this when the loop starts
        //myservo.write(parkAngle);
        //myservo.writeMicroseconds(parkAngle);

        // 1500 middle
        // 1000 left
        // 2000 right
        //servo.writeMicroseconds(1500);
        
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

void moveMotor(int targetPos, int currentPos, boolean stopMotion){
//  Serial.print("moveMotor");
//  Serial.print(targetPos);
//  Serial.print(" : ");
//  Serial.print(currentPos);
  
  if(stopMotion){
    dir = 0;
  } else if(currentPos == targetPos || currentPos == targetPos + 1 || currentPos == targetPos - 1) {
    Serial.print(" - targetHit - ");
    dir = 0;
    targetHit = true;
  } else if(currentPos < targetPos || currentPos < limits[0]){
    dir = -1; 
  } else if(currentPos > targetPos || currentPos > limits[1]){
    dir = 1;
  }

  Serial.print(" - ");  
  Serial.print(dir);
  
  if(dir == 0){
    // no motion
    analogWrite(pinPwm, 0);
    digitalWrite(pinDir, LOW);
  } else if(dir == 1) {
    // outward
    Serial.print(" - outward");
    analogWrite(pinPwm, 255); 
    digitalWrite(pinDir, LOW);
  } else if(dir == -1) {
    // inward
    Serial.print(" - inward");
    analogWrite(pinPwm, 255);
    digitalWrite(pinDir, HIGH);
  }
  
  Serial.println();
}

void checkBeforeMove(int positionTarget, int potPosition, boolean stopMotion){
  if(lastStatePos != positionTarget){
    targetHit = false; 
  }
  lastStatePos = positionTarget;
  if(!targetHit){
    moveMotor(positionTarget, potPosition, false);
  }
}

void loop() 
{ 
  int potPosition =  analogRead(potPos);
  parkState = digitalRead(parkPin);
  reverseState = digitalRead(reversePin);
  neutralState = digitalRead(neutralPin);
  driveState = digitalRead(drivePin);
  lowState = digitalRead(lowPin);
  
  if(parkState == 1){
    Serial.print("parkState");
    checkBeforeMove(parkPos, potPosition, false);
  } else if(reverseState == 1){
    Serial.print("reverseState");
    checkBeforeMove(reversePos, potPosition, false);
  } else if(neutralState == 1){
    Serial.print("neutralState");
    checkBeforeMove(neutralPos, potPosition, false);
  } else if(driveState == 1){
    Serial.print("driveState");
    checkBeforeMove(drivePos, potPosition, false);
  } else if(lowState == 1){
    Serial.print("lowState");
    checkBeforeMove(lowPos, potPosition, false);
  } else {
    // stop motor
    moveMotor(limits[0], potPosition, true);
  }
  
  delay(20);
	// refresh angle
//        int angle = myservo.read();
//        parkState = digitalRead(parkPin);
//        reverseState = digitalRead(reversePin);
//        neutralState = digitalRead(neutralPin);
//        driveState = digitalRead(drivePin);
//        lowState = digitalRead(lowPin);
        
//        Serial.println("park: "+(String)parkState+" reverse: "+(String)reverseState+" neutral: "+(String)neutralState+" drive: "+(String)driveState+" low: "+(String)lowState);
//
//        if(parkState == HIGH){
//          servoAngle = parkAngle;
//        } else if(reverseState == HIGH){
//          servoAngle = reverseAngle;
//        } else if(neutralState == HIGH){
//          servoAngle = neutralAngle;
//        } else if(driveState == HIGH){
//          servoAngle = driveAngle;
//        } else if(lowState == LOW){
//          //servoAngle = lowAngle;
//        }
//        if(servoAngle > -1){
//          if(!servoAttached){
//            // attaches the servo on pin to the servo object
//	    myservo.attach(servoPin);
//            servoAttached = true;
//          }
//          myservo.write(servoAngle);
//          //myservo.writeMicroseconds(servoAngle);
//        }
} 
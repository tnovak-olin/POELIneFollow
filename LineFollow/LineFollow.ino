//MARK: import libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>

//MARK: initalize variables

//left sensor value at port A1
int leftSensorValue;
//right sensor value at port A0
int rightSensorValue; 

//the sensor reading which corrosponds to the sensor being on or off the line
int offL;
int onL;
int offR;
int onR;

//a number describing what percent of the maximum speed the fast wheel should hit
float spd;

//the motor speed component variables
float wheelSpd;

//the motor speed variables
float leftWheelSpeed;
float rightWheelSpeed;

//calibration variables
float leftMultiple;

//serial input variables
char character;
String string;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

//set up the motors
Adafruit_DCMotor *leftWheel = AFMS.getMotor(1);
Adafruit_DCMotor *rightWheel = AFMS.getMotor(2);

void setup() {
  
  //the sensor reading which corrosponds to the sensor being on or off the line
  offL = 810;
  onL = 900;
  offR = 435;
  onR = 745;
  
  //a multiplier to slow down the left wheel to track at the same speed as the right wheel. (determined experimentally)
  leftMultiple = .6;
  //set default string and character values
  character = 0;
  string = "";

  //begin serial and motor controller
  Serial.begin(9600);
  AFMS.begin();

}

void loop() {
  //update the variable values

  //if a character is available in the serial window
  if (Serial.available() > 0) {
    // get the character
    character = Serial.read();  
         
    //if the character is not newline character
    if (character != '\n') {
      //append to the string
      string += character;
    }
    else {

      //update the speed
      spd = string.toFloat();
      string = "";
    }
  }
  
  //MARK: update variables
  wheelSpd = spd * 110.0 * (255.0/100.0);

  //MARK: read the sensor values
  leftSensorValue = analogRead(A5);
  rightSensorValue = analogRead(A0);

  //MARK: line following decision tree.
  if (leftSensorValue >= onL && rightSensorValue <= offR){
    //turnLeft  
    //start the wheels driving the right forward and the left backwards
    leftWheel->run(FORWARD);
    rightWheel->run(BACKWARD);

  }
  else if (leftSensorValue <= offL && rightSensorValue >= onR){
    //turnleft
    //start the wheels driving the right backwards and the left forward
    leftWheel->run(BACKWARD);
    rightWheel->run(FORWARD);
  }
  else{
    //go straight
    //start the wheels driving both wheels forward
    leftWheel->run(BACKWARD);
    rightWheel->run(BACKWARD);
    wheelSpd = spd * 65.5 * (255.0/100.0);
  }

  leftWheelSpeed = wheelSpd*leftMultiple; 
  rightWheelSpeed = wheelSpd;

  leftWheel->setSpeed(leftWheelSpeed);
  rightWheel->setSpeed(rightWheelSpeed);

  Serial.print(" LW: ");
  Serial.print(leftWheelSpeed);
  Serial.print(" LS: ");
  Serial.print(leftSensorValue);
  Serial.print(" RW: ");
  Serial.print(rightWheelSpeed);
  Serial.print(" RS: ");
  Serial.println(rightSensorValue);
  
  //send the signal to the wheel
  leftWheel->run(RELEASE);
  rightWheel->run(RELEASE);

}

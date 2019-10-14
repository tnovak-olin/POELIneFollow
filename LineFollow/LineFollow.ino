#include <Wire.h>
#include <Adafruit_MotorShield.h>

 //left sensor value at port A1
int leftSensorValue;
//right sensor value at port A0
int rightSensorValue; 

//the sensor reading which corrosponds to the sensor being on or off the line
int white;
int black;

//a number describing what percent of the maximum speed the fast wheel should hit
float spd;

//the motor speed variables
float high;
float low;

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
  // put your setup code here, to run once:
  //Set up the sensor reading channels

  //the sensor reading which corrosponds to the sensor being on or off the line
  white = 400;
  black = 700;
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

  //print the current speed to the terminal
  Serial.println(spd);
  
  //update wheel speeds
  high = spd * 100.0 * (255.0/100.0);
  low = spd * 50.0 * (255.0/100.0);

  //read the sensor values
  leftSensorValue = analogRead(A1);
  rightSensorValue = analogRead(A0);

  
  
  // put your main code here, to run repeatedly:
  leftWheel->run(BACKWARD);
  rightWheel->run(BACKWARD);

  //MARK: line following decision tree.
  if (leftSensorValue >= black && rightSensorValue <= white){
    //turnLeft
    leftWheel->setSpeed(low*leftMultiple);
    rightWheel->setSpeed(high);
  }
  else if (leftSensorValue <= white && rightSensorValue >= black){
    //turnright
    leftWheel->setSpeed(high*leftMultiple);
    rightWheel->setSpeed(low);
  }
  else{
    //go straight
    leftWheel->setSpeed(high*leftMultiple);
    rightWheel->setSpeed(high);
  }
    
  //send the signal to the wheel
  leftWheel->run(RELEASE);
  rightWheel->run(RELEASE);

}

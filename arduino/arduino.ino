#include <Arduino.h>

#define RIGHT_PWM 9
#define RIGHT_IN_A 3
#define RIGHT_IN_B 4
#define RIGHT_ENC_A 7
#define RIGHT_ENC_B 13

#define LEFT_PWM 5
#define LEFT_IN_1 8
#define LEFT_IN_2 6
#define LEFT_ENC_A 10
#define LEFT_ENC_B 11

//Define the Pins

//Motor 1
int pinAIN1 = 3; //Direction
int pinAIN2 = 4; //Direction
int pinPWMA = 9; //Speed

//Motor 2
int pinBIN1 = 6; //Direction
int pinBIN2 = 8; //Direction
int pinPWMB = 5; //Speed

//Standby
int pinSTBY = 2;

//Constants to help remember the parameters
static boolean turnCW = 0;  //for motorDrive function
static boolean turnCCW = 1; //for motorDrive function
static boolean motor1 = 0;  //for motorDrive, motorStop, motorBrake functions
static boolean motor2 = 1;  //for motorDrive, motorStop, motorBrake functions

void motorDrive(boolean motorNumber, boolean motorDirection, int motorSpeed)
{
  /*
  This Drives a specified motor, in a specific direction, at a specified speed:
    - motorNumber: motor1 or motor2 ---> Motor 1 or Motor 2
    - motorDirection: turnCW or turnCCW ---> clockwise or counter-clockwise
    - motorSpeed: 0 to 255 ---> 0 = stop / 255 = fast
  */

  boolean pinIn1;  //Relates to AIN1 or BIN1 (depending on the motor number specified)

 
//Specify the Direction to turn the motor
  //Clockwise: AIN1/BIN1 = HIGH and AIN2/BIN2 = LOW
  //Counter-Clockwise: AIN1/BIN1 = LOW and AIN2/BIN2 = HIGH
  if (motorDirection == turnCW)
    pinIn1 = HIGH;
  else
    pinIn1 = LOW;

//Select the motor to turn, and set the direction and the speed
  if(motorNumber == motor1)
  {
    digitalWrite(pinAIN1, pinIn1);
    digitalWrite(pinAIN2, !pinIn1);  //This is the opposite of the AIN1
    analogWrite(pinPWMA, motorSpeed);
  }
  else
  {
    digitalWrite(pinBIN1, pinIn1);
    digitalWrite(pinBIN2, !pinIn1);  //This is the opposite of the BIN1
    analogWrite(pinPWMB, motorSpeed);
  }
   
 

//Finally , make sure STBY is disabled - pull it HIGH
  digitalWrite(pinSTBY, HIGH);

}

void motorBrake(boolean motorNumber)
{
/*
This "Short Brake"s the specified motor, by setting speed to zero
*/

  if (motorNumber == motor1)
    analogWrite(pinPWMA, 0);
  else
    analogWrite(pinPWMB, 0);
   
}


void motorStop(boolean motorNumber)
{
  /*
  This stops the specified motor by setting both IN pins to LOW
  */
  if (motorNumber == motor1) {
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, LOW);
  }
  else
  {
    digitalWrite(pinBIN1, LOW);
    digitalWrite(pinBIN2, LOW);
  } 
}


void motorsStandby()
{
  /*
  This puts the motors into Standby Mode
  */
  digitalWrite(pinSTBY, LOW);
}



void setup()
{
//Set the PIN Modes
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinAIN2, OUTPUT);

  pinMode(pinPWMB, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);

  pinMode(pinSTBY, OUTPUT);

}

void loop()
{
 
  //Drive both motors CW, full speed
  motorDrive(motor1, turnCW, 255);
  motorDrive(motor2, turnCW, 255);
 
  //Keep driving for 2 secs
  delay(2000);

  //Turn towards motor1: Stop Motor1, slow Motor2
  motorStop(motor1);
  motorDrive(motor2, turnCW, 192);
 
  //Keep turning for 2 secs
  delay(2000);

  //Turn in opposite direction: Stop Motor2, slow Motor1
  motorDrive(motor1, turnCW, 192);
  delay(250);
  motorStop(motor2);

  //Keep turning for 2 secs
  delay(2000);

  //Straighten up
  motorDrive(motor2, turnCW, 192);
  delay(500);
 
  //Put motors into Standby
  motorsStandby();
  delay(1000);
 
  //Do a tight turn towards motor1: Motor2 forward, Motor1 reverse
  motorDrive(motor1, turnCCW, 192);
  motorDrive(motor2, turnCW, 192);
 
  //Keep turning for 2 secs
  delay(2000);
 

  //Apply Brakes, then into Standby
  motorBrake(motor1);
  motorBrake(motor2);
  motorsStandby();

  //Stand still for 5 secs, then we do it all over again...
  delay(5000);

}

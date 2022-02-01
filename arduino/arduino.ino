#include <Arduino.h>
#include <Wire.h> 

#define CW 0
#define CCW 1
#define RIGHT 0 
#define LEFT 1

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

//Standby
int pinSTBY = 2;

bool lastLeftState = false;
bool lastRightState = false;
int counterRight = 0;
int counterLeft = 0;

void motorDrive(boolean motorNumber, boolean motorDirection, int motorSpeed)
{
  /*
  This Drives a specified motor, in a specific direction, at a specified speed:
    - motorNumber: RIGHT or LEFT ---> Motor 1 or Motor 2
    - motorDirection: CW or CCW ---> clockwise or counter-clockwise
    - motorSpeed: 0 to 255 ---> 0 = stop / 255 = fast
  */

  boolean pinIn1;  //Relates to AIN1 or BIN1 (depending on the motor number specified)

 
//Specify the Direction to turn the motor
  //Clockwise: AIN1/BIN1 = HIGH and AIN2/BIN2 = LOW
  //Counter-Clockwise: AIN1/BIN1 = LOW and AIN2/BIN2 = HIGH
  if (motorDirection == CW)
    pinIn1 = HIGH;
  else
    pinIn1 = LOW;

//Select the motor to turn, and set the direction and the speed
  if(motorNumber == RIGHT)
  {
    digitalWrite(RIGHT_IN_A, pinIn1);
    digitalWrite(RIGHT_IN_B, !pinIn1);  //This is the opposite of the AIN1
    analogWrite(RIGHT_PWM, motorSpeed);
  }
  else
  {
    digitalWrite(LEFT_IN_1, pinIn1);
    digitalWrite(LEFT_IN_2, !pinIn1);  //This is the opposite of the BIN1
    analogWrite(LEFT_PWM, motorSpeed);
  }
   
 

//Finally , make sure STBY is disabled - pull it HIGH
  digitalWrite(pinSTBY, HIGH);

}

void motorBrake(boolean motorNumber)
{
/*
This "Short Brake"s the specified motor, by setting speed to zero
*/

  if (motorNumber == RIGHT)
    analogWrite(RIGHT_PWM, 0);
  else
    analogWrite(LEFT_PWM, 0);
   
}

void motorStop(boolean motorNumber)
{
  /*
  This stops the specified motor by setting both IN pins to LOW
  */
  if (motorNumber == RIGHT) {
    digitalWrite(RIGHT_IN_A, LOW);
    digitalWrite(RIGHT_IN_B, LOW);
  }
  else
  {
    digitalWrite(LEFT_IN_1, LOW);
    digitalWrite(LEFT_IN_2, LOW);
  } 
}

void encoderRead(int pin_a, int pin_b, boolean *lastEncoderState, int *encoderCount)
{
  bool currentEncoderState = digitalRead(pin_a);
  if (currentEncoderState != *lastEncoderState)
  {
    *lastEncoderState = currentEncoderState;
    if (digitalRead(pin_b) != currentEncoderState)
      (*encoderCount)++;
    else
      (*encoderCount)--;
  }
}

void encoderReset(int *encoderCount)
{
  *encoderCount = 0;
}

void requestEvent() {
    char command = Wire.read();
    Serial.print("Request: ");
    Serial.println(command);
  
}

void receiveEvent(int bytes) {
    Serial.print("Receive: ");
    Serial.println(bytes);
  
}


void setup(){
  
  Wire.begin(0x76);            
  Wire.onRequest(requestEvent); // respond to requests from the master nodde via read8
  Wire.onReceive(receiveEvent); // receive data from the master node via write8

  Serial.begin(9600);
  delay(2000);
  Serial.println("Arduino is ready");


  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_IN_A, OUTPUT);
  pinMode(RIGHT_IN_B, OUTPUT);

  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_IN_1, OUTPUT);
  pinMode(LEFT_IN_2, OUTPUT);

  pinMode(pinSTBY, OUTPUT);

}

void loop()
{
  //encoderRead(RIGHT_ENC_A, RIGHT_ENC_B, &lastRightState, &counterRight);
  //encoderRead(LEFT_ENC_A, LEFT_ENC_B, &lastLeftState, &counterLeft);
}


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

int right_pwm, right_in_a, right_in_b, right_enc_a, right_enc_b;
int left_pwm, left_in_a, left_in_b, left_enc_a, left_enc_b; 
int right_speed, right_direction, right_state;
int left_speed, left_direction, left_state;

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
    digitalWrite(right_in_a, pinIn1);
    digitalWrite(right_in_b, !pinIn1);  //This is the opposite of the AIN1
    analogWrite(right_pwm, motorSpeed);
  }
  else
  {
    digitalWrite(left_in_a, pinIn1);
    digitalWrite(left_in_b, !pinIn1);  //This is the opposite of the BIN1
    analogWrite(left_pwm, motorSpeed);
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
    digitalWrite(right_in_a, LOW);
    digitalWrite(right_in_b, LOW);
  }
  else
  {
    digitalWrite(left_in_a, LOW);
    digitalWrite(left_in_b, LOW);
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

void configurePins(int index, int value) {
   if (index == 0x00) {
        pinMode(value, OUTPUT);
        right_in_a = value;
        Serial.println("> pins > Right IN A: " + String(value));

    } else if (index == 0x01) {
        pinMode(value, OUTPUT);
        right_in_b = value;
        Serial.println("> pins > Right IN B: " + String(value));

    } else if (index == 0x02) {
        pinMode(value, OUTPUT);
        right_pwm = value;
        Serial.println("> pins > Right PWM: " + String(value));

    } else if (index == 0x03) {
        pinMode(value, INPUT);
        right_enc_a = value;
        Serial.println("> pins > Right ENC A: " + String(value));

    } else if (index == 0x04) {
        pinMode(value, INPUT);
        right_enc_b = value;
        Serial.println("> pins > Right ENC B: " + String(value));

    } else if (index == 0x05) {
        pinMode(value, OUTPUT);
        left_in_a = value;
        Serial.println("> pins > Left IN A: " + String(value));

    } else if (index == 0x06){
        pinMode(value, OUTPUT);
        left_in_b = value;
        Serial.println("> pins > Left IN B: " + String(value));

    } else if (index == 0x07) {
        pinMode(value, OUTPUT);
        left_pwm = value;
        Serial.println("> pins > Left PWM: " + String(value));

    } else if (index == 0x0A) {
        pinMode(value, INPUT);
        left_enc_a = value;
        Serial.println("> pins > Left ENC A: " + String(value));

    } else if (index == 0x0B) {
        pinMode(value, INPUT);
        left_enc_b = value;
        Serial.println(" > pins > Left ENC B: " + String(value));
    } 
}

void setMotors(int index, int value) {
  if (index == 0x10) {
    right_speed = value;
    Serial.println("> motors > Right Speed: " + String(value));

  } else if (index == 0x11) {
    right_direction = value;
    Serial.println("> motors > Right Direction: " + String(value));

  } else if (index == 0x12) {
    right_state = value;
    Serial.println("> motors > Right State " + String(value));
  
  }else if (index == 0x20) {
    left_speed = value;
    Serial.println("> motors > Left Speed: " + String(value));

  } else if (index == 0x21) {
    left_direction = value;
    Serial.println("> motors > Left Direction: " + String(value));

  } else if (index == 0x22) {
    right_state = value;
    Serial.println("> motors > Left State: " + String(value));
  }
}

void updateMotors(){
  if (right_state == 1){ 
    if(right_direction == 1){
      motorDrive(RIGHT, CW, right_speed);
    }else{
      motorDrive(RIGHT, CCW, right_speed);
    }
  }else{
    motorStop(RIGHT);
  }

  if(left_state == 1){
    if(left_direction == 1){
      motorDrive(LEFT, CW, left_speed);
    }else{
      motorDrive(LEFT, CCW, left_speed);
    }
  }else{
    motorStop(LEFT);
  }
}

void receiveEvent(int bytes) {
    int index = Wire.read();
    int value = Wire.read();
    int checksum = Wire.read();

    configurePins(index, value);
    setMotors(index, value);
}

void setup(){
  Serial.begin(9600);
  
  Wire.begin(0x76);            
  Wire.onRequest(requestEvent); // respond to requests from the master nodde via read8
  Wire.onReceive(receiveEvent); // receive data from the master node via write8

  Serial.println("Arduino is ready");
}

void loop()
{
  updateMotors();
  //encoderRead(RIGHT_ENC_A, RIGHT_ENC_B, &lastRightState, &counterRight);
  //encoderRead(LEFT_ENC_A, LEFT_ENC_B, &lastLeftState, &counterLeft);
}


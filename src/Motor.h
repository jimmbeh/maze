// Include Arduino standard library to access standard functions
#include <Arduino.h>

// Speed control pins
#define motorRSpeed 9
#define motorLSpeed 10

// Direction control pins
#define motorL2 A3
#define motorL1 A2
#define motorR1 A1
#define motorR2 A0

const int error = 4;

// Stop motor
void restMotor(){
    analogWrite(motorLSpeed,0);
    analogWrite(motorRSpeed,0);
    digitalWrite(motorL1,LOW);
    digitalWrite(motorL2,LOW);
    digitalWrite(motorR1,LOW);
    digitalWrite(motorR2,LOW);
}

// Motor moving forward
void goForwardMotor(int speed){
    digitalWrite(motorL1,LOW);
    digitalWrite(motorL2,HIGH);
    digitalWrite(motorR1,LOW);
    digitalWrite(motorR2,HIGH);
    analogWrite(motorLSpeed,speed-error);
    analogWrite(motorRSpeed,speed+error);
}

// Motor moving rightwards
void goRightMotor(int speed){
    digitalWrite(motorL1,LOW);
    digitalWrite(motorL2,HIGH);
    digitalWrite(motorR1,LOW);
    digitalWrite(motorR2,LOW);
    analogWrite(motorLSpeed,speed);
    analogWrite(motorRSpeed,0);
}

// Motor moving rightwards hard
void goHardRightMotor(int speed){
    digitalWrite(motorL1,LOW);
    digitalWrite(motorL2,HIGH);
    digitalWrite(motorR1,HIGH);
    digitalWrite(motorR2,LOW);
    analogWrite(motorLSpeed,speed);
    analogWrite(motorRSpeed,speed);
}

// Motor moving leftwards
void goLeftMotor(int speed){
    digitalWrite(motorL1,LOW);
    digitalWrite(motorL2,LOW);
    digitalWrite(motorR1,LOW);
    digitalWrite(motorR2,HIGH);
    analogWrite(motorLSpeed,0);
    analogWrite(motorRSpeed,speed);
}

// Motor moving leftwards hard
void goHardLeftMotor(int speed){
    digitalWrite(motorL1,HIGH);
    digitalWrite(motorL2,LOW);
    digitalWrite(motorR1,LOW);
    digitalWrite(motorR2,HIGH);
    analogWrite(motorLSpeed,speed);
    analogWrite(motorRSpeed,speed);
}

// Motor part setup code
void motorSetup(){
    pinMode(motorLSpeed, OUTPUT);
    pinMode(motorRSpeed, OUTPUT);
    pinMode(motorL1, OUTPUT);
    pinMode(motorL2, OUTPUT);
    pinMode(motorR1, OUTPUT);
    pinMode(motorR2, OUTPUT);
    restMotor();
}
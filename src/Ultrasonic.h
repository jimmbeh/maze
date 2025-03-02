// Include Arduino standard library to access standard functions
#include <Arduino.h>

// Front ultrasonic sensor
#define frontTrig 12
#define frontEcho 13

// Right ultrasonic sensor
#define rightTrig 8
#define rightEcho 11

// Left ultrasonic sensor
#define leftTrig 7
#define leftEcho 6

// Ultrasonic part setup code
void ultrasonicSetup(){
    pinMode(frontEcho, INPUT);
    pinMode(frontTrig, OUTPUT);
    pinMode(rightEcho, INPUT);
    pinMode(rightTrig, OUTPUT);
    pinMode(leftEcho, INPUT);
    pinMode(leftTrig, OUTPUT);
}

// Return distance in cm
float getDistanceFront(){
    digitalWrite(frontTrig,LOW);
    delay(2);
    digitalWrite(frontTrig,HIGH);
    delay(10);
    digitalWrite(frontTrig,LOW);
    return (pulseIn(frontEcho,HIGH) * 0.034613 / 2.00);
}

// Return distance in cm
float getDistanceRight(){
    digitalWrite(rightTrig,LOW);
    delay(2);
    digitalWrite(rightTrig,HIGH);
    delay(10);
    digitalWrite(rightTrig,LOW);
    return (pulseIn(rightEcho,HIGH) * 0.034613 / 2.00);
}

// Return distance in cm
float getDistanceLeft(){
    digitalWrite(leftTrig,LOW);
    delay(2);
    digitalWrite(leftTrig,HIGH);
    delay(10);
    digitalWrite(leftTrig,LOW);
    return (pulseIn(leftEcho,HIGH) * 0.034613 / 2.00);
}
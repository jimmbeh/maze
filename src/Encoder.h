// Include Arduino standard library to access standard functions
#include <Arduino.h>

#define encoderPinL 2
#define encoderPinR 3

volatile unsigned long pulsesL = 0;
volatile unsigned long pulsesR = 0;

void counterUpdateL()
{
    pulsesL++;
}

void counterUpdateR()
{
    pulsesR++;
}

void encoderSetup(){
  // Set encoder pins as input with pull-up resistors
  pinMode(encoderPinL, INPUT); 
  pinMode(encoderPinR, INPUT); 

  // Attach interrupts to the encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderPinL), counterUpdateL, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinR), counterUpdateR, RISING);
}

// This equation is slightly different from what can be obtained online.
// 0.05 means 1/20 as there are 20 slots on the disc
// 0.0325 metre is the wheel radius
// 100 means result in cm
// however actual circumference should be 2*PI*R instead of PI*R
const float wheelCircumference = 0.05 * 2 * PI * 0.0325 * 100;

float getMovingDistanceL(){
    noInterrupts();

    // TIPS: ((pulsesLeft + pulsesRight) / 2) * wheelCircumference seems ok to use it
    float distance = pulsesL * wheelCircumference;

    //Restart the interrupt processing
    interrupts();

    return distance;
}

float getMovingDistanceR(){
    noInterrupts();

    // TIPS: ((pulsesLeft + pulsesRight) / 2) * wheelCircumference seems ok to use it
    float distance = pulsesR * wheelCircumference;

    //Restart the interrupt processing
    interrupts();

    return distance;
}

void resetDistance(){
    pulsesL = 0;
    pulsesR = 0;
}

// How to use?
// encoderSetup(); in setup

// Serial.println(String(getMovingDistance())); to access the distance travel
// resetDistance(); once you want to reset the distance counter
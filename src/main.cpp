#include <Arduino.h>
#include <Memory.h>
#include <Motor.h>
#include <Ultrasonic.h>
#include <Encoder.h>
#include <Gyro.h>

void goStraight(float targetDistance){
  float initialDistance = getMovingDistance();
  float initialLeftDistance = getMovingDistanceLeft();
  float initialRightDistance = getMovingDistanceRight();
  while ((getMovingDistance()-initialDistance)<targetDistance) {
    float movedLeft = getMovingDistanceLeft() - initialLeftDistance;
    float movedRight = getMovingDistanceRight() - initialRightDistance;
    if ((movedLeft-movedRight)>0.5) {
      slightLeft();
    } else if ((movedRight-movedLeft)>0.5) {
      slightRight();
    } else {
        goForwardMotor(120);
    }
  }
  restMotor();
}

void goStraightGyro(float targetDistance){
  mpuSetup();
  float initialDistance = getMovingDistance();
  while ((getMovingDistance()-initialDistance)<targetDistance) {
    if (update()<179.9) {
      slightLeft();
    } else if (update()>180.1) {
      slightRight();
    } else {
        goForwardMotor(120);
    }
  }
  restMotor();
}

void turnRight(){
  float targetAngle = update() - 90;
  if (targetAngle<0){
      targetAngle += 360;
  }
  while (abs(update()-targetAngle)>25){
      goHardRightMotor(120);
  }
  restMotor();
}

void turnLeft(){
  float targetAngle = update() + 90;
  if (targetAngle>360){
      targetAngle -= 360;
  }
  while (abs(update()-targetAngle)>25){
      goHardLeftMotor(120);
  }
  restMotor();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  motorSetup();
  ultrasonicSetup();
  encoderSetup();
  mpuSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  float fx = getDistanceFront();
  float rx = getDistanceRight();
  float lx = getDistanceLeft();
  // Serial.println(String(fx)); Serial.println(String(rx)); Serial.println(String(lx));

  while (abs(lx-rx)>1) { // while left and right distance, difference >1
    if (lx>rx) { // if far from left wall
      slightLeft();
    } else if (rx>lx) { // if far from right wall
      slightRight();
    }
    lx = getDistanceLeft(); // refresh left distance
    rx = getDistanceRight(); // refresh right distance
  }

  if (fx<5) { // if too near to front wall
    turnRight(); delay(500);
  } else if (lx>25) { // else if no left wall
    turnLeft(); delay(500);
  } else {
    goStraightGyro(5);
  }
}

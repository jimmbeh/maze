#include <Arduino.h>
#include <Memory.h>
#include <Motor.h>
#include <Ultrasonic.h>
#include <Encoder.h>
#include <Gyro.h>

int const dfSpeed = 110;  // default speed
int const slSpeed = 120;  // slight speed
int const htSpeed = 110;  // hard turn speed
int orT = 20; // start orientation, facing front (180d)
int const frontLm = 6;
int const sideLm = 8;
int const sideDt = 25;

int checkORT() {
  int currentAngle;
  if ((orT%4)==0) {currentAngle=180;}
  else if (((orT+1)%4)==0) {currentAngle=90;}
  else if (((orT-1)%4)==0) {currentAngle=270;}
  else if (((orT+2)%4)==0) {currentAngle=360;}
  return(currentAngle);
}

void slightRight(){ // robot to tilt right, use when adjusting position slightly
  goRightMotor(slSpeed);
  // delay(2);
  // restMotor();
}

void slightLeft(){  // robot to tilt left, use when adjusting position slightly
  goLeftMotor(slSpeed);
  // delay(2);
  // restMotor();
}

void goStraightGyro(float targetDistance, int currentAngle){  // robot to go straight for specified distance based on gyroscope feedback
  targetDistance = 3.7* targetDistance;
  float initialDistance = getMovingDistance();
  while ((getMovingDistance()-initialDistance)<targetDistance) {
    if (update()<currentAngle) {
      slightLeft();
    } else if (update()>currentAngle) {
      slightRight();
    } else {
        goForwardMotor(dfSpeed);
    }
  }
  restMotor();
}

void goStraightUS(float targetDistance) {
  targetDistance = 3.7* targetDistance;
  float initialDistance = getMovingDistance();
  float lx = getDistanceLeft();
  float rx = getDistanceRight();
  while ((getMovingDistance()-initialDistance)<targetDistance) {
    lx = getDistanceLeft();
    rx = getDistanceRight();
    if (lx>rx) {
      slightLeft();
    } else if (rx>lx) {
      slightRight();
    } else {
        goForwardMotor(dfSpeed);
    }
  }
  restMotor();
}

void turnRight(int currentAngle){ // turn sharp right (-90d)
  float targetAngle = currentAngle - 90;
  if (targetAngle<0){
      targetAngle += 360;
  }
  while (abs(update()-targetAngle)>30){
      goHardRightMotor(htSpeed);
  }
  orT -= 1;
  restMotor();
  // delay(1000);
}

void turnLeft(int currentAngle){  // turn sharp left (+90d)
  float targetAngle = currentAngle + 90;
  if (targetAngle>360){
      targetAngle -= 360;
  }
  while (abs(update()-targetAngle)>30){
      goHardLeftMotor(htSpeed);
  }
  orT += 1;
  restMotor(); 
  // delay(1000);
}

void testSensor() { // test gyroscope, encoder, and ultrasonic using serial monitor
  float fx = getDistanceFront();
  float rx = getDistanceRight();
  float lx = getDistanceLeft();
  Serial.print("Front Distance: "); Serial.println(String(fx)); 
  Serial.print("Right Distance: "); Serial.println(String(rx)); 
  Serial.print("Left Distance: "); Serial.println(String(lx));
  Serial.print("Yaw Angle: "); Serial.println(String(update()));
  Serial.print("Moved Distance Left: "); Serial.println(String(getMovingDistanceLeft()));
  Serial.print("Moved Distance Right: "); Serial.println(String(getMovingDistanceRight()));
  Serial.print("Moved Distance Average: "); Serial.println(String(getMovingDistance()));
}

void testMotor() {  // test robot movement in 1s intervals, fwd, stop, left, stop, fwd, right, stop
  goForwardMotor(dfSpeed); delay(1000);
  restMotor(); delay(1000);
  goLeftMotor(dfSpeed); delay(1000);
  restMotor(); delay(1000);
  goForwardMotor(dfSpeed); delay(1000);
  restMotor(); delay(1000);
  goRightMotor(dfSpeed); delay(1000);
  restMotor(); delay(1000);
}

void testAdvanced() { // test advanced movement in 1s intervals, goFwd 100cm, turn right, turn left
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  turnRight(checkORT()); 
  // delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  turnRight(checkORT()); 
  // delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  turnLeft(checkORT()); 
  // delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  turnLeft(checkORT()); delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
  goStraightGyro(25, checkORT()); 
  // delay(1000);
}

void followLeftWall() {
  float fx = getDistanceFront();
  float lx = getDistanceLeft();
  float rx = getDistanceRight();
  if (fx<frontLm) {
    fx = getDistanceFront();
    if (fx<frontLm) {
      turnRight(checkORT());
    }
  } else if (lx>sideDt) {
    lx = getDistanceLeft();
    if (lx>sideDt) {
      turnLeft(checkORT());
    }
  } else if (lx<sideDt && rx<sideDt) {
    goStraightUS(1);
  } else {
    goStraightGyro(1, checkORT());
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  motorSetup();
  ultrasonicSetup();
  encoderSetup();
  mpuSetup();

  // testMotor();
  testAdvanced();
}

void loop() {
  // put your main code here, to run repeatedly:
  // testSensor();

  // followLeftWall();
}

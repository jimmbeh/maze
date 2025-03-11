#include <Arduino.h>
#include <Memory.h>
#include <Motor.h>
#include <Ultrasonic.h>
#include <Encoder.h>
#include <Gyro.h>

int const dfSpeed = 120;

void slightRight(){ // robot to tilt right, use when adjusting position slightly
  goRightMotor(dfSpeed+20);
  delay(5);
  restMotor();
}

void slightLeft(){  // robot to tilt left, use when adjusting position slightly
  goLeftMotor(dfSpeed+20);
  delay(5);
  restMotor();
}

void goStraightGyro(float targetDistance, float currentAngle){  // robot to go straight for specified distance based on gyroscope feedback
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

void turnRight(){ // turn sharp right (-90d)
  float targetAngle = update() - 90;
  if (targetAngle<0){
      targetAngle += 360;
  }
  while (abs(update()-targetAngle)>25){
      goHardRightMotor(120);
  }
  restMotor();
}

void turnLeft(){  // turn sharp left (+90d)
  float targetAngle = update() + 90;
  if (targetAngle>360){
      targetAngle -= 360;
  }
  while (abs(update()-targetAngle)>25){
      goHardLeftMotor(120);
  }
  restMotor();
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
  testMotor();
  delay(10000000);
}

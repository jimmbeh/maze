#include <Arduino.h>
#include <Memory.h>
#include <Motor.h>
#include <Ultrasonic.h>
#include <Encoder.h>
#include <Gyro.h>

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
}

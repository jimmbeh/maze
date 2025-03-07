// Include Arduino standard library to access standard functions
#include <Arduino.h>
// Include Wire library for I2C communication
#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address

// Pin connection
// VCC -> 5 V
// GND -> GND 
// SCL -> A5 
// SDA -> A4 

const float GYRO_SCALE = 1.0 / 131.0;
float gyroOutputBuffer = 0;

// Get orientation value from MPU6050
void getOrientation(){

    // Reset variable holder for X, Y
    gyroOutputBuffer = 0;

    Wire.beginTransmission(MPU);
    Wire.write(0x47);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 2, true);
    gyroOutputBuffer = (Wire.read() << 8 | Wire.read()) * GYRO_SCALE;  // get z rotation (yaw)
    // Serial.println(String(gyroOutputBuffer));
}


float GyroErrorZ;

void calculateError() {
    // Read accelerometer values 200 times
    byte c = 0;
    GyroErrorZ = 0;
    
    // Read gyro values 200 times
    while (c < 200) {
        getOrientation();
        GyroErrorZ += gyroOutputBuffer;
        c++;
    }
    
    //Divide the sum by 200 to get the error value
    GyroErrorZ = GyroErrorZ / 200;
    Serial.println("The gryoscope setting in MPU6050 has been calibrated");
}


// MPU6050 part setup code
void mpuSetup(){
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  
  // Call this function if you need to get the IMU error values for your module
  calculateError();
}


float elapsedTime, previousTime;
float currentTime = millis();
float yaw;
volatile float angle = 0;

float update(){
    // === Read gyroscope (on the MPU6050) data === //
    previousTime = currentTime;
    currentTime = millis();
    elapsedTime = (currentTime - previousTime) * 0.001; // Divide by 1000 to get seconds
    getOrientation();
    // Correct the outputs with the calculated error values
    gyroOutputBuffer -= GyroErrorZ;
    // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
    yaw += gyroOutputBuffer * elapsedTime;
    angle = round(yaw*10)/10.0; //if you mounted MPU6050 in a different orientation to me, angle may not = roll. It can roll, pitch, yaw or minus version of the three
    //for me, turning right reduces angle. Turning left increases angle.
    if (angle>180){
        angle = angle - abs(int(angle/180)) * 360 + abs(int(angle/360)) * 360;
    }
    else if (angle < -180){
        angle = angle + abs(int(angle/180)) * 360 - abs(int(angle/360)) * 360;
    }
    angle +=180;
    return(angle);
}

// HOW to use?
// mpuSetup(); in setup

// update(); in loop
// Serial.println(String(angle)); to access the angle
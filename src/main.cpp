#include <Arduino.h>  // Include Arduino core library
#include <Memory.h>   // Include memory management library
#include <Motor.h>    // Include motor control library
#include <Ultrasonic.h>  // Include ultrasonic sensor library
#include <Encoder.h>  // Include encoder library
#include <Gyro.h>     // Include gyroscope library

int const dfSpeed = 115;  // Default speed for motors
int const slSpeed = dfSpeed+10;  // Slight turn speed (default + 10)
int const htSpeed = dfSpeed+30;  // Hard turn speed (default + 30)
int referenceAngle = 180;  // Reference angle for gyroscope (initial facing direction)
int const frontLm = 6;  // Front sensor limit (unused in this script)
int const sideLm = 10;  // Side sensor limit (unused in this script)
int const sideDt = 20;  // Side distance threshold for maze mapping

void slightRight() {  // Adjust robot slightly to the right
  goRightMotor(slSpeed);  // Move right motor at slight speed
}

void slightLeft() {  // Adjust robot slightly to the left
  goLeftMotor(slSpeed);  // Move left motor at slight speed
}

void goStraightGyro(float targetDistance) {  // Move robot straight for a specific distance
  float initialDistance = getMovingDistance();  // Get initial distance from encoder
  while ((getMovingDistance()-initialDistance)<targetDistance) {  // Loop until target distance is reached
    if (referenceAngle==0) {  // If reference angle is 0 (facing right)
      if (update()<180) {  // If current angle is less than 180
        slightRight();  // Adjust slightly right
      } else {
        slightLeft();  // Adjust slightly left
      }
    } else if (update()<referenceAngle) {  // If current angle is less than reference
      slightLeft();  // Adjust slightly left
    } else if (update()>referenceAngle) {  // If current angle is greater than reference
      slightRight();  // Adjust slightly right
    } else {
        goForwardMotor(dfSpeed);  // Move forward at default speed
    }
    delay(20);  // Small delay for stability
    restMotor();  // Stop motors briefly
    delay(5);  // Small delay
  } 
  restMotor();  // Stop motors after movement
  delay(500);  // Delay to stabilize
}

void turnRight() {  // Turn robot 90 degrees to the right
  referenceAngle = (referenceAngle + 270) % 360;  // Update reference angle for right turn
  while (abs(update()-referenceAngle)>5){  // Loop until angle is within 5 degrees of target
      goHardRightMotor(htSpeed);  // Turn right at hard turn speed
      delay(20);  // Small delay
      restMotor();  // Stop motors briefly
      delay(10);  // Small delay
  }
  restMotor();  // Stop motors after turn
  delay(500);  // Delay to stabilize
}

void turnLeft() {  // Turn robot 90 degrees to the left
  referenceAngle = (referenceAngle + 90) % 360;  // Update reference angle for left turn
  while (abs(update()-referenceAngle)>5){  // Loop until angle is within 5 degrees of target
      goHardLeftMotor(htSpeed);  // Turn left at hard turn speed
      delay(20);  // Small delay
      restMotor();  // Stop motors briefly
      delay(10);  // Small delay
  }
  restMotor();  // Stop motors after turn
  delay(500);  // Delay to stabilize
}

void testSensor() {  // Test sensors and print data to serial monitor
  float fx = getDistanceFront();  // Get front ultrasonic distance
  float rx = getDistanceRight();  // Get right ultrasonic distance
  float lx = getDistanceLeft();  // Get left ultrasonic distance
  Serial.print("Front Distance: "); Serial.println(String(fx));  // Print front distance
  Serial.print("Right Distance: "); Serial.println(String(rx));  // Print right distance
  Serial.print("Left Distance: "); Serial.println(String(lx));  // Print left distance
  Serial.print("Yaw Angle: "); Serial.println(String(update()));  // Print gyroscope angle
  Serial.print("Moved Distance Left: "); Serial.println(String(getMovingDistanceLeft()));  // Print left encoder distance
  Serial.print("Moved Distance Right: "); Serial.println(String(getMovingDistanceRight()));  // Print right encoder distance
  Serial.print("Moved Distance Average: "); Serial.println(String(getMovingDistance()));  // Print average encoder distance
}

void testMotor() {  // Test motor movements in 1-second intervals
  goForwardMotor(dfSpeed); delay(1000);  // Move forward for 1 second
  restMotor(); delay(1000);  // Stop for 1 second
  goLeftMotor(dfSpeed); delay(1000);  // Turn left for 1 second
  restMotor(); delay(1000);  // Stop for 1 second
  goForwardMotor(dfSpeed); delay(1000);  // Move forward for 1 second
  restMotor(); delay(1000);  // Stop for 1 second
  goRightMotor(dfSpeed); delay(1000);  // Turn right for 1 second
  restMotor(); delay(1000);  // Stop for 1 second
}

void testAdvanced() {  // Test advanced movements (forward, turns, etc.)
  goStraightGyro(25);  // Move forward 25 units
  turnRight();  // Turn right
  goStraightGyro(25);  // Move forward 25 units
  goStraightGyro(25);  // Move forward 25 units
  turnRight();  // Turn right
  goStraightGyro(25);  // Move forward 25 units
  turnLeft();  // Turn left
  goStraightGyro(25);  // Move forward 25 units
  turnLeft();  // Turn left
  goStraightGyro(25);  // Move forward 25 units
  goStraightGyro(25);  // Move forward 25 units
  goStraightGyro(25);  // Move forward 25 units
  goStraightGyro(25);  // Move forward 25 units
}

#define N 8  // Maze size (8x8 grid)
#define GRID_SIZE (2*N+1)  // Grid size for maze (2N+1)
#define MAX_STEPS 20  // Maximum steps for maze mapping
char maze[GRID_SIZE][GRID_SIZE];  // 2D array to represent the maze
int posX = 16, posY = 1;  // Starting position in the maze grid
int steps = 0;  // Step counter for maze mapping

void initializeMaze() {  // Initialize maze grid with walls ('#')
  for (int i = 0; i < GRID_SIZE; i++)
      for (int j = 0; j < GRID_SIZE; j++)
          maze[i][j] = '#';  // Fill maze with walls
}

void updateMaze() {  // Mark current position in the maze as visited ('.')
  maze[posX][posY] = '.';  // Mark current position as visited
}

void moveForward() {  // Move robot forward in the maze
  goStraightGyro(25);  // Move forward 25 units
  if (referenceAngle == 180) posX--;  // Update X position if facing north
  if (referenceAngle ==  90) posY++;  // Update Y position if facing east
  if (referenceAngle ==   0) posX++;  // Update X position if facing south
  if (referenceAngle == 270) posY--;  // Update Y position if facing west
  if (posX<0 || posY<0 || posX>=GRID_SIZE || posY>=GRID_SIZE) steps=MAX_STEPS;  // Stop if out of bounds
  updateMaze();  // Update maze grid
  steps++;  // Increment step counter
}

void printMaze() {  // Print the maze grid to the serial monitor
  for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {
          Serial.print(maze[i][j]);  // Print each cell
          Serial.print(" ");  // Add space for readability
      }
      Serial.println();  // New line for each row
  }
}

void mapMaze() {  // Map the maze using sensor data
  moveForward();  // Move forward initially
  while (steps < MAX_STEPS) {  // Loop until max steps are reached
      if (getDistanceLeft() > sideDt) {  // If left side is open
          turnLeft(); moveForward(); moveForward();  // Turn left and move forward
      } else if ((getDistanceFront() > sideDt) && (getDistanceFront() < 1000)) {  // If front is open
          moveForward(); moveForward();  // Move forward
      } else if (getDistanceRight() > sideDt) {  // If right side is open
          turnRight(); moveForward(); moveForward();  // Turn right and move forward
      } else {  // If no path is open
          turnRight(); turnRight();  // Turn 180 degrees
      }
      printMaze();  // Print maze after each move
  }
}

void setup() {  // Arduino setup function
  Serial.begin(115200);  // Initialize serial communication
  motorSetup();  // Initialize motors
  ultrasonicSetup();  // Initialize ultrasonic sensors
  encoderSetup();  // Initialize encoders
  mpuSetup();  // Initialize gyroscope
  initializeMaze();  // Initialize maze grid
  mapMaze();  // Start maze mapping
}

void loop() {  // Arduino loop function
  // Uncomment to test sensors
  // testSensor();
}
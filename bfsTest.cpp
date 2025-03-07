#include <Arduino.h>
#include <QueueArray.h>
#include <Ultrasonic.h>
#include <Motor.h>
#include <opencv2/opencv.hpp> // For simulation (install OpenCV for visualization)

using namespace std;
using namespace cv;

// Define the dimensions of the maze
const int MAZE_WIDTH = 5;
const int MAZE_HEIGHT = 5;

// Define the maze
char maze[MAZE_WIDTH][MAZE_HEIGHT];

// Create a structure to represent a point in the maze
struct Point {
    int x;
    int y;
};

// Directions for moving in the maze (up, right, down, left)
int directions[4][2] = {
    {-1, 0}, // up
    {0, 1},  // right
    {1, 0},  // down
    {0, -1}  // left
};

// Initialize the sensors and motors (for Arduino)
Ultrasonic ultrasonicFront(7, 8); // Example pins for ultrasonic sensor facing front
Ultrasonic ultrasonicLeft(9, 10); // Example pins for ultrasonic sensor facing left
Ultrasonic ultrasonicRight(11, 12); // Example pins for ultrasonic sensor facing right
Motor motorLeft(3, 4, 5); // Example pins for motor control
Motor motorRight(6, 7, 8); // Example pins for motor control

// Function to print the maze
void printMaze(char maze[MAZE_WIDTH][MAZE_HEIGHT]) {
    for (int i = 0; i < MAZE_WIDTH; i++) {
        for (int j = 0; j < MAZE_HEIGHT; j++) {
            Serial.print(maze[i][j]);
            Serial.print(' ');
        }
        Serial.println();
    }
}

// Function to print the path
void printPath(Point parent[MAZE_WIDTH][MAZE_HEIGHT], Point start, Point end) {
    Point current = end;
    while (!(current.x == start.x && current.y == start.y)) {
        Serial.print("(");
        Serial.print(current.x);
        Serial.print(", ");
        Serial.print(current.y);
        Serial.print(") ");
        current = parent[current.x][current.y];
    }
    Serial.print("(");
    Serial.print(start.x);
    Serial.print(", ");
    Serial.print(start.y);
    Serial.println(")");
}

// BFS function to find the shortest path
bool bfs(Point start, Point end, char maze[MAZE_WIDTH][MAZE_HEIGHT], Point parent[MAZE_WIDTH][MAZE_HEIGHT], bool visited[MAZE_WIDTH][MAZE_HEIGHT]) {
    QueueArray<Point> queue;
    queue.enqueue(start);
    visited[start.x][start.y] = true;

    while (!queue.isEmpty()) {
        Point current = queue.dequeue();

        // Check if we reached the end
        if (current.x == end.x && current.y == end.y) {
            return true;
        }

        // Explore the neighbors
        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i][0];
            int newY = current.y + directions[i][1];

            // Check if the new position is valid
            if (newX >= 0 && newX < MAZE_WIDTH && newY >= 0 && newY < MAZE_HEIGHT && maze[newX][newY] != '#' && !visited[newX][newY]) {
                visited[newX][newY] = true;
                queue.enqueue({newX, newY});
                parent[newX][newY] = current;
            }
        }
    }

    // No path found
    return false;
}

// Function to update the maze based on sensor readings (for Arduino)
void updateMaze(Point current) {
    // Check front sensor
    long distanceFront = ultrasonicFront.readDistance();
    if (distanceFront < 20) {
        maze[current.x + directions[0][0]][current.y + directions[0][1]] = '#';
    } else {
        maze[current.x + directions[0][0]][current.y + directions[0][1]] = '.';
    }

    // Check left sensor
    long distanceLeft = ultrasonicLeft.readDistance();
    if (distanceLeft < 20) {
        maze[current.x + directions[3][0]][current.y + directions[3][1]] = '#';
    } else {
        maze[current.x + directions[3][0]][current.y + directions[3][1]] = '.';
    }

    // Check right sensor
    long distanceRight = ultrasonicRight.readDistance();
    if (distanceRight < 20) {
        maze[current.x + directions[1][0]][current.y + directions[1][1]] = '#';
    } else {
        maze[current.x + directions[1][0]][current.y + directions[1][1]] = '.';
    }
}

// Function to move the micromouse to a specific point (for Arduino)
void moveTo(Point point) {
    motorLeft.moveForward(200);
    motorRight.moveForward(200);
    delay(1000); // Example delay, adjust based on your motor speed and distance
    motorLeft.stop();
    motorRight.stop();
    delay(500); // Example delay for stopping
}

// Function to create animation (for simulation)
void createAnimation(char maze[MAZE_WIDTH][MAZE_HEIGHT], const vector<Point>& path) {
    int cellSize = 30; // Size of each cell in pixels
    Mat img(MAZE_HEIGHT * cellSize, MAZE_WIDTH * cellSize, CV_8UC3, Scalar(255, 255, 255));

    // Draw maze
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            if (maze[i][j] == '#') {
                rectangle(img, Point(j * cellSize, i * cellSize), Point((j + 1) * cellSize, (i + 1) * cellSize), Scalar(0, 0, 0), FILLED);
            }
        }
    }

    // Draw path
    for (const auto& p : path) {
        rectangle(img, Point(p.y * cellSize, p.x * cellSize), Point((p.y + 1) * cellSize, (p.x + 1) * cellSize), Scalar(0, 255, 0), FILLED);
    }

    // Save animation
    imwrite("maze_solution.png", img);
    imshow("Maze Solution", img);
    waitKey(0);
}

// Main function for Arduino
void runArduino() {
    Serial.begin(9600);

    // Initialize the maze with empty cells
    for (int i = 0; i < MAZE_WIDTH; i++) {
        for (int j = 0; j < MAZE_HEIGHT; j++) {
            maze[i][j] = '.';
        }
    }

    // Set the start and end points
    Point start = {0, 0};
    Point end = {4, 4};
    maze[start.x][start.y] = 'S';
    maze[end.x][end.y] = 'E';

    // Perform BFS to find the shortest path
    Point parent[MAZE_WIDTH][MAZE_HEIGHT];
    bool visited[MAZE_WIDTH][MAZE_HEIGHT] = {false};
    if (bfs(start, end, maze, parent, visited)) {
        // Reconstruct and print the path
        Serial.println("Path found:");
        printPath(parent, start, end);
    } else {
        Serial.println("No path found!");
    }
}

// Main function for simulation
void runSimulation() {
    // Initialize the maze with empty cells
    for (int i = 0; i < MAZE_WIDTH; i++) {
        for (int j = 0; j < MAZE_HEIGHT; j++) {
            maze[i][j] = '.';
        }
    }

    // Set the start and end points
    Point start = {0, 0};
    Point end = {4, 4};
    maze[start.x][start.y] = 'S';
    maze[end.x][end.y] = 'E';

    // Perform BFS to find the shortest path
    Point parent[MAZE_WIDTH][MAZE_HEIGHT];
    bool visited[MAZE_WIDTH][MAZE_HEIGHT] = {false};
    if (bfs(start, end, maze, parent, visited)) {
        // Reconstruct the path
        vector<Point> path;
        Point current = end;
        while (!(current.x == start.x && current.y == start.y)) {
            path.push_back(current);
            current = parent[current.x][current.y];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());

        // Create animation
        createAnimation(maze, path);
    } else {
        cout << "No path found!" << endl;
    }
}

// Choose which version to run
int main() {
    // Uncomment the function you want to run
    // runArduino(); // For Arduino
    runSimulation(); // For simulation

    return 0;
}
#include "../include/main.h"

// Port definitions
#define LEFT_WHEEL_1 1
#define LEFT_WHEEL_2 2
#define LEFT_WHEEL_3 3
#define RIGHT_WHEEL_1 4
#define RIGHT_WHEEL_2 7
#define RIGHT_WHEEL_3 8
#define FLYWHEEL 9
#define RIGHT_ARM 10
#define FORK 11
#define INTAKE 14
#define IMU 20

// Motor / sensor declarations
extern pros::Motor leftWheel1;
extern pros::Motor leftWheel2;
extern pros::Motor leftWheel3;
extern pros::Motor rightWheel1;
extern pros::Motor rightWheel2;
extern pros::Motor rightWheel3;
extern pros::Motor flywheel;
extern pros::Motor intake;
extern pros::Controller master;

extern pros::ADIEncoder leftQuad;
extern pros::ADIEncoder rightQuad;
extern pros::ADIEncoder middleQuad;
extern pros::Imu imu;

// PID / Odom functions
void moveDistance(double distanceInFeet);
void turnAngle(double angleInDegrees);

// Utility functions
void clearLCDLines();
float sign(float input);
float signChecker(float input, float sameSign);
float limit(float input, float max);
std::string fixFileName(std::string name);
void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<float>>> dataset);
std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename);
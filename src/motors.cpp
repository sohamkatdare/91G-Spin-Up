#include "../include/main.h"

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

pros::Motor leftWheel1 (LEFT_WHEEL_1);
pros::Motor leftWheel2 (LEFT_WHEEL_2);
pros::Motor leftWheel3(LEFT_WHEEL_3);
pros::Motor rightWheel1 (RIGHT_WHEEL_1, true);
pros::Motor rightWheel2 (RIGHT_WHEEL_2, true);
pros::Motor rightWheel3 (RIGHT_WHEEL_3, true);
pros::Motor flywheel (FLYWHEEL);
pros::Motor intake (INTAKE);
pros::Controller master (CONTROLLER_MASTER);

pros::ADIEncoder leftQuad ('A', 'B');
pros::ADIEncoder rightQuad ('C', 'D');
pros::ADIEncoder middleQuad ('E', 'F');
pros::Imu imu (IMU);
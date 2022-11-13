#include "../include/main.h"
#include "initialize.hpp"

pros::Motor leftWheel1 (LEFT_WHEEL_1);
pros::Motor leftWheel2 (LEFT_WHEEL_2);
pros::Motor leftWheel3 (LEFT_WHEEL_3);
pros::Motor rightWheel1 (RIGHT_WHEEL_1, true);
pros::Motor rightWheel2 (RIGHT_WHEEL_2, true);
pros::Motor rightWheel3 (RIGHT_WHEEL_3, true);
pros::Motor flywheel (FLYWHEEL);
pros::Motor intake (INTAKE);
pros::Controller master (CONTROLLER_MASTER);

pros::ADIEncoder leftQuad(LEFT_QUAD_TOP, LEFT_QUAD_BOTTOM, true);
pros::ADIEncoder rightQuad(RIGHT_QUAD_TOP, RIGHT_QUAD_BOTTOM);
pros::Imu imu (IMU);
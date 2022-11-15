#ifndef _INIT_H_
#define _INIT_H_

#include "../include/main.h"

//Defintions for Ports
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

#define LEFT_QUAD_TOP 'A'
#define LEFT_QUAD_BOTTOM 'B'
#define RIGHT_QUAD_TOP 'C'
#define RIGHT_QUAD_BOTTOM 'D'
#define INDEXER 'e'

//Forward Definition for PROS Components
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
extern pros::ADIDigitalOut indexer;


#endif

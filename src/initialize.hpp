#ifndef _INIT_H_
#define _INIT_H_

#include "../include/main.h"
#include "pros/optical.hpp"

//Defintions for Ports
#define LEFT_WHEEL_1 12
#define LEFT_WHEEL_2 13
#define LEFT_WHEEL_3 2
#define RIGHT_WHEEL_1 14
#define RIGHT_WHEEL_2 15
#define RIGHT_WHEEL_3 8
#define FLYWHEEL 1
#define INTAKE 7
#define IMU 20

#define LEFT_QUAD_TOP 'A'
#define LEFT_QUAD_BOTTOM 'H'
#define RIGHT_QUAD_TOP 'G'
#define RIGHT_QUAD_BOTTOM 'D'
#define EXTENSION1 'B'
#define EXTENSION2 'C'

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
extern pros::Optical diskSensor;
extern pros::ADIDigitalOut indexer;
extern pros::ADIDigitalOut extension1;
extern pros::ADIDigitalOut extension2;

void m_intake(bool direction);

#endif

#ifndef _INIT_H_
#define _INIT_H_

#include "../include/main.h"
#include "pros/optical.hpp"

// Defintions for Ports
#define LEFT_WHEEL_1 17
#define LEFT_WHEEL_2 12
#define LEFT_WHEEL_3 11
#define RIGHT_WHEEL_1 13
#define RIGHT_WHEEL_2 10
#define RIGHT_WHEEL_3 18
#define FLYWHEEL 1
#define INTAKE 11
#define IMU 20
// #define DISK 2

#define LEFT_QUAD_TOP 'A'
#define LEFT_QUAD_BOTTOM 'B'
#define RIGHT_QUAD_TOP 'C'
#define RIGHT_QUAD_BOTTOM 'D'
#define INDEXER 'G'
#define EXTENSION 'H'

// Forward Definition for PROS Components
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
extern pros::ADIDigitalOut extension;

void m_intake(bool direction);

#endif

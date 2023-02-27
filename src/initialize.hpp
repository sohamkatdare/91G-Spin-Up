#ifndef _INIT_H_
#define _INIT_H_

#include "../include/main.h"
#include "pros/adi.hpp"
#include "pros/optical.hpp"

// Defintions for Ports
#define LEFT_WHEEL_1 11
#define LEFT_WHEEL_2 4
#define LEFT_WHEEL_3 3
#define RIGHT_WHEEL_1 1
#define RIGHT_WHEEL_2 5
#define RIGHT_WHEEL_3 7
#define CATA 8
#define INTAKE 10
#define IMU 20

// #define LEFT_QUAD_TOP 'A'
// #define LEFT_QUAD_BOTTOM 'H'
// #define RIGHT_QUAD_TOP 'G'
// #define RIGHT_QUAD_BOTTOM 'D'
#define LIMIT 'G'
#define EXTENSION 'H'


// Forward Definition for PROS Components
extern pros::Motor leftWheel1;
extern pros::Motor leftWheel2;
extern pros::Motor leftWheel3;
extern pros::Motor rightWheel1;
extern pros::Motor rightWheel2;
extern pros::Motor rightWheel3;
extern pros::Motor cata;
extern pros::Motor intake;
extern pros::Controller master;

extern pros::ADIEncoder leftQuad;
extern pros::ADIEncoder rightQuad;
extern pros::ADIEncoder middleQuad;
extern pros::Imu imu;
extern pros::Optical diskSensor;
extern pros::ADIDigitalOut indexer;
extern pros::ADIDigitalOut extension;
extern pros::ADIDigitalIn cataLimit;

void m_intake(bool direction);

#endif

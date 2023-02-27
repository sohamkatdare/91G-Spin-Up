#include "../include/main.h"
#include "initialize.hpp"
#include "pid.hpp"

//Constants
#define ANALOG_MAX 127
#define ANALOG_MIN -127
#define ANALOG_ZERO 0

void drive_move(double left_voltage, double right_voltage) {
	rightWheel1.move(right_voltage);
	rightWheel2.move(right_voltage);
	rightWheel3.move(right_voltage);
	leftWheel1.move(left_voltage);
	leftWheel2.move(left_voltage);
	leftWheel3.move(left_voltage);
}

// void flywheel_move() {
//     int flywheelSpeed = 0;
//     int flywheelSlew = 20;
//     if (flywheel.get_actual_velocity() == 0) {
//         while (flywheelSpeed < ANALOG_MAX) {
//             flywheelSpeed = ((flywheelSpeed + flywheelSlew) > ANALOG_MAX ? ANALOG_MAX : (flywheelSpeed + flywheelSlew));
//         }
//     } else {
//         flywheelSpeed = ((flywheelSpeed - flywheelSlew) < ANALOG_ZERO ? ANALOG_ZERO : (flywheelSpeed - flywheelSlew));
//     }
// }

void roller() {
    intake.move(-127);
	drive_move(40, 40);
    pros::delay(500);
	intake.move(0);
	drive_move(0, 0);
	pros::delay(2000);
	drive_move(-40, -40);
	pros::delay(200);
	drive_move(0, 0);
}

void basket() {
	roller();
	// flywheel_move();
	intake.move(-127);
	pros::delay(500);
	intake.move(0);
	pros::delay(1000);
	intake.move(-127);
	pros::delay(500);
	intake.move(0);
}

void progSkills() {
	
}

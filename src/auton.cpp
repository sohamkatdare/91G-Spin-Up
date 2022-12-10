#include "../include/main.h"
#include "initialize.hpp"
#include "pid.hpp"

//Constants
#define ANALOG_MAX 127
#define ANALOG_MIN -127
#define ANALOG_ZERO 0

void shoot_disc(int discnum) { // number of discs in container
	for (int i = 0; i < discnum; i++) {
		indexer.set_value(true);
		pros::delay(20);
		indexer.set_value(false);
		pros::delay(20);
	}
}

void flywheel_move() {
    int flywheelSpeed = 0;
    int flywheelSlew = 20;
    if (flywheel.get_actual_velocity() == 0) {
        while (flywheelSpeed < ANALOG_MAX) {
            flywheelSpeed = ((flywheelSpeed + flywheelSlew) > ANALOG_MAX ? ANALOG_MAX : (flywheelSpeed + flywheelSlew));
        }
    } else {
        flywheelSpeed = ((flywheelSpeed - flywheelSlew) < ANALOG_ZERO ? ANALOG_ZERO : (flywheelSpeed - flywheelSlew));
    }
}

void soloAWP() {
    intake.move(-127);
		rightWheel1.move(40);
		rightWheel2.move(40);
		rightWheel3.move(40);
		leftWheel1.move(40);
		leftWheel2.move(40);
		leftWheel3.move(40);
    pros::delay(500);
		intake.move(0);
		rightWheel1.move(0);
		rightWheel2.move(0);
		rightWheel3.move(0);
		leftWheel1.move(0);
		leftWheel2.move(0);
		leftWheel3.move(0);
		pros::delay(2000);
		rightWheel1.move(-40);
		rightWheel2.move(-40);
		rightWheel3.move(-40);
		leftWheel1.move(-40);
		leftWheel2.move(-40);
		leftWheel3.move(-40);
		pros::delay(200);
		rightWheel1.move(0);
		rightWheel2.move(0);
		rightWheel3.move(0);
		leftWheel1.move(0);
		leftWheel2.move(0);
		leftWheel3.move(0);
    // moveDistance(-1);
		// pros::delay(500);
		// rightWheel1.move(0);
		// rightWheel2.move(0);
		// rightWheel3.move(0);
		// leftWheel1.move(0);
		// leftWheel2.move(0);
		// leftWheel3.move(0);
    // turnAngle(-10);
    // flywheel_move();
    // shoot_disc(2);
    // turnAngle(-75);
    // moveDistance(6);
    // turnAngle(45);
    // moveDistance(2);
    // turnAngle(45);
    // moveDistance(6);
    // turnAngle(90);
    // m_intake(true);
    // pros::delay(1000);
    // m_intake(true);
    // moveDistance(-1);
}

#include "../include/main.h"
#include "initialize.hpp"
#include "pid.hpp"

//Constants
#define ANALOG_MAX 127
#define ANALOG_MIN -127
#define ANALOG_ZERO 0

void m_intake(bool direction) { //true for intake, false for outtake
    if (intake.get_actual_velocity() == 0) {
        if (direction) {
            intake.move(ANALOG_MAX);
        } else {
            intake.move(ANALOG_MIN);
	    }
    } else {
        intake.move(ANALOG_ZERO);
    }
}

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
    int flywheelSlew = 15;
    if (flywheel.get_actual_velocity() == 0) {
        while (flywheelSpeed < ANALOG_MAX) {
            flywheelSpeed = ((flywheelSpeed + flywheelSlew) > ANALOG_MAX ? ANALOG_MAX : (flywheelSpeed + flywheelSlew));
        }
    } else {
        flywheelSpeed = ((flywheelSpeed - flywheelSlew) < ANALOG_ZERO ? ANALOG_ZERO : (flywheelSpeed - flywheelSlew));
    }
}

void soloAWP() {
    m_intake(true);
    pros::delay(1000);
    m_intake(true);
    moveDistance(-1);
    turnAngle(-15);
    flywheel_move();
    shoot_disc(2);
    turnAngle(-75);
    moveDistance(6);
    turnAngle(45);
    moveDistance(2);
    turnAngle(45);
    moveDistance(6);
    turnAngle(90);
    m_intake(true);
    pros::delay(1000);
    m_intake(true);
    moveDistance(-1);
}
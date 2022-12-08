#include "../include/main.h"
#include "../include/pros/misc.h"

#include "initialize.hpp"
#include "odom.hpp"
#include "pid.hpp"
#include "util.hpp"

//Constants
#define ANALOG_MAX 127
#define ANALOG_MIN -127
#define ANALOG_ZERO 0


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	pros::Task task {[=] {
		std::uint32_t now = pros::millis();
		while (true) {
			pros::Task::delay_until(&now, 16);
			odomCalculations();
		}
	}};
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	turnAngle(90);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	//Possibily Add Slew Control Override
	bool slewOverride = false; //Update this if needed.

	//Starting Motor Input Values
	int prevLeft = 0;
	int prevRight = 0;

	//Drive Slew Rate Controls
	int slewThreshold = 20; //Threshold at which slew rate is enabled.
	int slew = 25; //Rate at which to slew. Possibily implement this as max rate for slewing.

	//Bools for Intake and Flywheel. Change Defaults as needed.
	bool intakeOn = false;
	bool flywheelOn = false;

	//Intake Stall Mechanism
	bool intakeStall = false; //Implement to enable automatic intake disloge possibily.

	//Flywheel Slew rate
	int flywheelSpeed = 0;
	int flywheelSlew = 15;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		//Drive Control
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		int lOutput = left;
		if (abs(left - prevLeft) > slewThreshold) {
			//Change too large. Enable Slew Rate.
			int change = slew * ((left-prevLeft) >= 0 ? 1: -1);
			lOutput = prevLeft + change;
			//Normalize Slew Output.
			if (lOutput > ANALOG_MAX){
				lOutput = ANALOG_MAX;
			} else if (lOutput < ANALOG_MIN) {
				lOutput = ANALOG_MIN;
			}
		}
		leftWheel1.move(lOutput);
		leftWheel2.move(lOutput);
		leftWheel3.move(lOutput);

		int rOutput = right;
		if (abs(right - prevRight) > slewThreshold) {
			//Change too large. Enable Slew Rate.
			int change = slew * ((right-prevRight) >= 0 ? 1: -1);
			rOutput = prevRight + change;
			//Normalize Slew Output.
			if (rOutput > ANALOG_MAX){
				rOutput = ANALOG_MAX;
			} else if (rOutput < ANALOG_MIN) {
				rOutput = ANALOG_MIN;
			}
		}
		rightWheel1.move(rOutput);
		rightWheel2.move(rOutput);
		rightWheel3.move(rOutput);

		prevLeft = lOutput;
		prevRight = rOutput;

		//Intake and Roller Control
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			//Toggle intake state.
			intakeOn = !intakeOn;
		}
		if (intakeOn) {
			intake.move(ANALOG_MAX); //Change speed if this is too fast.
		} else {
			intake.move(ANALOG_ZERO);
		}
		//Add Roller Macro.

		//Flywheel Control
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			//Toggle flywheel state.
			flywheelOn = !flywheelOn;
		}
		if (flywheelOn){
			flywheelSpeed = ((flywheelSpeed + flywheelSlew) > ANALOG_MAX ? ANALOG_MAX : (flywheelSpeed + flywheelSlew));
		} else {
			flywheelSpeed = ((flywheelSpeed - flywheelSlew) < ANALOG_ZERO ? ANALOG_ZERO : (flywheelSpeed - flywheelSlew));
		}

		flywheel.move(flywheelSpeed);

		pros::delay(20);

		// Indexer Control
		if (master.get_digital(DIGITAL_L2)) {
			indexer.set_value(true);
			pros::delay(500);
			indexer.set_value(false);
		}

	}
}

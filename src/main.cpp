#include "../include/main.h"
#include "../include/pros/misc.h"

#include "auton.hpp"
#include "initialize.hpp"
#include "odom.hpp"
#include "pid.hpp"
#include "util.hpp"

// Constants
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

  // pros::Task task {[=] {
  // 	std::uint32_t now = pros::millis();
  // 	while (true) {
  // 		pros::Task::delay_until(&now, 16);
  // 		odomCalculations();
  // 	}
  // }};
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
  pros::lcd::set_text(3, "Auton");
  // turnAngle(90);
  // moveDistance(2);
  roller();
  // while(true){
  // 	pros::lcd::set_text(4, std::to_string(leftQuad.get_value()));
  // 	pros::lcd::set_text(5, std::to_string(rightQuad.get_value()));
  // 	pros::delay(20);
  // }
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
  pros::lcd::set_text(3, "Driver Control");

  // Possibly Add Slew Control Override
  bool slewOverride = false; // Update this if needed.

  // Starting Motor Input Values
  int prevLeft = 0;
  int prevRight = 0;

  // Drive Slew Rate Controls
  int slewThreshold = 20; // Threshold at which slew rate is enabled.
  int slew = 25; // Rate at which to slew. Possibily implement this as max rate
                 // for slewing.

  // Bools for Roller, Intake, and Flywheel. Change Defaults as needed.
  bool rollerOn = false;
  bool intakeOn = false;
  bool intakeReversed = false;
  bool fire = false;
  bool cataFire = false;

  // Intake Stall Mechanism
  bool intakeStall = false; // Implement to enable automatic intake disloge possibily.

  // Toogle Button Bools
  bool aPressed = false;
  bool l1Pressed = false;
  bool l2Pressed = false;
  bool r1Pressed = false;
  bool r2Pressed = false;
  bool rightPressed = false;
  bool leftPressed = false;

  while (true) {
    // THIS CODE BLOCKS THE MAIN THREAD... DO THIS ASYNC...
    //  if (slewOverride) {
    //  	pros::delay(5000);
    //  	slewOverride = false;
    //  }

    // Drive Control
    int left = master.get_analog(ANALOG_LEFT_Y);
    int right = master.get_analog(ANALOG_RIGHT_Y);

    if (!slewOverride) { // normal slew control
      int lOutput = left;
      if (abs(left - prevLeft) > slewThreshold) {
        // Change too large. Enable Slew Rate.
        int change = slew * ((left - prevLeft) >= 0 ? 1 : -1);
        lOutput = prevLeft + change;
        // Normalize Slew Output.
        if (lOutput > ANALOG_MAX) {
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
        // Change too large. Enable Slew Rate.
        int change = slew * ((right - prevRight) >= 0 ? 1 : -1);
        rOutput = prevRight + change;
        // Normalize Slew Output.
        if (rOutput > ANALOG_MAX) {
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
    }

    // Intake and Roller Control (Roller takes precedence over Intake)
    if (!aPressed && master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      // Toggle slow roller state.
      rollerOn = !rollerOn;
    }
    aPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);

    if (!l1Pressed && master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      // Toggle intake state.
      intakeOn = !intakeOn;
    }
    l1Pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_L1);

    if (!l2Pressed && master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      // Toggle intake state.
      intakeReversed = !intakeReversed;
    }
    l2Pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    if (rollerOn) {
      intake.move_velocity(-300);
    } else if (intakeOn) {
      if (intakeReversed) {
        intake.move_velocity(-600); // Change speed if this is too fast.
      } else {
        intake.move_velocity(600);
      }
    } else {
      intake.move_velocity(0);
    }
    // Add Roller Macro.

    // Intake Stall mechanism
    // if(intake.get_target_velocity() != 0 && intake.get_actual_velocity() ==
    // 0){ // If the motor's desired rotation speed is non-zero but it is stuck
    // 	double prev_vel = intake.get_target_velocity();
    // 	intake.move_velocity(-prev_vel); // reverse motor direction
    // 	pros::delay(2000); // allow some time to dislodge disk
    // 	intake.move_velocity(prev_vel); // return to previous direction
    // }

    // Cata Control
    if (!r1Pressed && master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { //Cata is assumed to be in the down position (loaded position).
      if (!cataFire){ //If cata is not firing, then fire it.
        cataFire = true; //Set cataFire flag.
      }
    }
    if (cataFire) { //When cataFire flag is set, cata is fired.
      cata.move(ANALOG_MAX); //Move continuously until next limit switch press.
      if (!fire) { //Cata is not slipping yet.
        if (!cataLimit.get_value()) { // Cata just slipped. Stop firing state.
          fire = true;
        }
      } else { //Cata Slip Gear slipping. 
        if (cataLimit.get_value()) { // Cata is still not fully loaded. Drawing back...
          cata.move(0); //Stop reloading. Fire on next R1 press.
          fire = false; //Reset fire flag.
          cataFire = false; //Reset cataFire flag.
        }
      }
    }
    r1Pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_R1);

    // Extension Control
    if (!rightPressed && master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      // extension1.set_value(false);
      // extension2.set_value(false);
      extension1.set_value(true);
      extension2.set_value(true);
    }

    rightPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT);
    leftPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);

    pros::delay(20);
  }
}

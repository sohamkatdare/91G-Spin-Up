#include "../include/main.h"
#include "initialize.hpp"
#include "util.hpp"

#define DIAMETER 3.25 // diameter of tracking wheels in inches
#define PI 3.14159

// drive pid

void moveDistance(double distanceInFeet) { //Sychronous function. Will block main thread.
	pros::lcd::set_text(5, "Outside");
	int quadratureTPR = 360;
	double circumference = DIAMETER * PI;
	double distanceInInches = distanceInFeet * 12;
	double targetTickTravel = quadratureTPR * (distanceInInches / circumference);
	pros::lcd::set_text(2, std::to_string(imu.get_heading()));
	pros::lcd::set_text(3, std::to_string(leftQuad.get_value()));
	pros::lcd::set_text(4, std::to_string(rightQuad.get_value()));

	//Constants
	double dkp = 0.15;
	double dki = 0;
	double dkd = 0;

	double skp = 1;
	double ski = 0;
	double skd = 0;

	double dTarget = (((double) (leftQuad.get_value() + rightQuad.get_value()))/2) + targetTickTravel; //Signifies the average tick position given the distance needed
	double dErrorSigned = targetTickTravel;
	double dError = std::abs(dErrorSigned);
	double dLastError = dError;
	double dIntegral = 0;
	int motionProfile = 0;

	double sTarget = imu.get_heading(); //Signifies the difference between the encoders at the start
	double sError = 0;
	double sLastError = sError;
	double sLastErrorSigned = sError;
	double sIntegral = 0;

	float currentTime = 0;
	std::vector<float> timeData;
	std::vector<float> leftData;
	std::vector<float> rightData;
	std::vector<float> sErrorData;
	std::vector<float> dErrorData;
	std::vector<float> dTargetData;
	std::vector<float> sTargetData;
	std::vector<float> rightMotorData;
	std::vector<float> leftMotorData;
	std::vector<float> sPIDData;
	std::vector<float> spData;
	std::vector<float> siData;
	std::vector<float> sdData;
	std::vector<float> sDirectionData;
	std::vector<float> dPIDData;
	std::vector<float> motionProfileData;
	while ((dError >= 10 || sError >= 0.5) && currentTime <= (375 * std::abs(distanceInFeet))) { //2 PID Loop with Threshold
		pros::lcd::set_text(1, std::to_string(dErrorSigned));
		pros::lcd::set_text(2, std::to_string(imu.get_heading()));
		pros::lcd::set_text(3, std::to_string(leftQuad.get_value()));
		pros::lcd::set_text(4, std::to_string(rightQuad.get_value()));
		pros::lcd::set_text(6, "Inside");
		double average = ((double) (leftQuad.get_value() + rightQuad.get_value()))/2;
		dErrorSigned = dTarget-average;
		dError = std::abs(dErrorSigned);
		double sErrorSigned = sTarget - imu.get_heading();
		sErrorSigned = ( (std::abs(sErrorSigned - sLastError) < std::abs(sErrorSigned - sLastError + 360)) && (std::abs(sErrorSigned - sLastError) < std::abs(sErrorSigned - sLastError - 360)) ? sErrorSigned : ((std::abs(sErrorSigned - sLastError+360) < std::abs(sErrorSigned - sLastError)) && (std::abs(sErrorSigned - sLastError+360) < std::abs(sErrorSigned - sLastError - 360)) ? sErrorSigned - sLastError + 360 : sErrorSigned - sLastError - 360 ) );
		sError = std::abs(sErrorSigned);
		timeData.push_back(currentTime);
		leftData.push_back(leftQuad.get_value());
		rightData.push_back(rightQuad.get_value());
		sErrorData.push_back(sErrorSigned);
		dErrorData.push_back(dErrorSigned);
		dTargetData.push_back(dTarget);
		sTargetData.push_back(sTarget);
		currentTime += 20;

		//dPID Loop
		double dp = dError * dkp;
		dIntegral += dError;
		double di = dIntegral * dki;
		double dd = (dError - dLastError) * dkd; //Derivative times derivative gain constant
		dLastError = dError;
		double dpid = dp + di + dd;
		dPIDData.push_back(dpid);

		//sPID Loop
		double sp = sError * skp;
		if(sErrorSigned * sLastErrorSigned < 0){
			sIntegral = 0;
		}
		sIntegral += sErrorSigned;
		double si = sIntegral * ski;
		double sd = (sError - sLastError) * skd; //Derivative times derivative gain constant
		sLastError = sError;
		sLastErrorSigned = sErrorSigned;
		double spid = sp + si + sd;
		spData.push_back(sp);
		siData.push_back(si);
		sdData.push_back(sd);
		sPIDData.push_back(spid);
		motionProfileData.push_back(motionProfile);

		double power = motionProfile;
		if (motionProfile >= dpid) { //PID triggers slowdown
			power = dpid; //To just test straightening PID //Switch to dpid
		}
		int dDirection = (dErrorSigned > 0 ? 1: -1);
		double rightSideCorrection = 0;
		double leftSideCorrection = 0;
		int sDirection = (sErrorSigned > 0 ? 1 : (sErrorSigned < 0 ? -1 : 0));
		sDirectionData.push_back(sDirection);
		if (dDirection * sDirection == 1) { //Left Side is too far forward. spid will reduce motor speed on left side.
			rightSideCorrection = spid;
		}
		else if (dDirection * sDirection == -1) {
			leftSideCorrection = spid; //Right Side is too far forward. spid will reduce motor speed on right side.
		}
		//Run all motors straight using correction from straightening PID
		rightMotorData.push_back(power-rightSideCorrection);
		leftMotorData.push_back(power-leftSideCorrection);
		rightWheel1.move((power-rightSideCorrection) * dDirection);
		rightWheel2.move((power-rightSideCorrection) * dDirection);
		rightWheel3.move((power-rightSideCorrection) * dDirection);
		leftWheel1.move((power-leftSideCorrection) * dDirection);
		leftWheel2.move((power-leftSideCorrection) * dDirection);
		leftWheel3.move((power-leftSideCorrection) * dDirection);
		motionProfile += (motionProfile + 10 <= 127 ? 10 : 0);
		pros::lcd::set_text(3, std::to_string(dErrorSigned));
		pros::lcd::set_text(4, std::to_string(sErrorSigned));
		pros::delay(20);
	}
	//Reseting
	rightWheel1.move(0);
	rightWheel2.move(0);
	leftWheel1.move(0);
	leftWheel2.move(0);
	pros::lcd::set_text(6, "Finished");

	std::vector<std::pair<std::string, std::vector<float>>> data;


	data.push_back(std::make_pair("Time", timeData));
	data.push_back(std::make_pair("Left", leftData));
	data.push_back(std::make_pair("Right", rightData));
	data.push_back(std::make_pair("sError", sErrorData));
	data.push_back(std::make_pair("dError", dErrorData));
	data.push_back(std::make_pair("sTarget", sTargetData));
	data.push_back(std::make_pair("dTarget", dTargetData));
	data.push_back(std::make_pair("sPID", sPIDData));
	data.push_back(std::make_pair("sp", spData));
	data.push_back(std::make_pair("si", siData));
	data.push_back(std::make_pair("sd", sdData));
	data.push_back(std::make_pair("sDirection", sDirectionData));
	data.push_back(std::make_pair("dPID", dPIDData));
	data.push_back(std::make_pair("Motion Profile", motionProfileData));
	data.push_back(std::make_pair("Left Motor", leftMotorData));
	data.push_back(std::make_pair("Right Motor", rightMotorData));
	write_csv("/usd/PIDDriveData.csv", data);
}

// turn pid

void turnAngle(double angleInDegrees) { //Sychronous function. Will block main thread.
	//Constants
	double kp = 2.7;
	double ki = 0;
	double kd = 3;
	double targetAngle = imu.get_heading() + angleInDegrees;
	if(targetAngle >= 360){
		targetAngle-=360;
	}else if(targetAngle < 0){
		targetAngle+=360;
	}
	double signedError = angleInDegrees;
	double lastSignedError = signedError;
	double error = std::abs(signedError);
	double lastError = error;
	double integral = 0;

	int motionProfile = 0;

	float currentTime = 0;
	std::vector<float> timeData;
	std::vector<float> errorData;
	std::vector<float> targetData;
	std::vector<float> motorData;
	std::vector<float> PIDData;
	std::vector<float> motionProfileData;
	pros::lcd::set_text(4, std::to_string(targetAngle));
	pros::lcd::set_text(5, std::to_string(error));

	while(error>=0.5 && currentTime<=(std::abs(angleInDegrees)*9.5) + 350){ //PID loop with threshold //Add timeout if needed!!!
		pros::lcd::set_text(2, std::to_string(imu.get_heading()));
		pros::lcd::set_text(3, std::to_string(leftQuad.get_value()));
		pros::lcd::set_text(4, std::to_string(rightQuad.get_value()));
		pros::lcd::set_text(5, std::to_string(targetAngle));

		signedError = targetAngle - imu.get_heading();
		signedError = ( (std::abs(signedError - lastSignedError) < std::abs(signedError - lastSignedError + 360)) && (std::abs(signedError - lastSignedError) < std::abs(signedError - lastSignedError - 360)) ? signedError : ((std::abs(signedError - lastSignedError+360) < std::abs(signedError - lastSignedError)) && (std::abs(signedError - lastSignedError+360) < std::abs(signedError - lastSignedError - 360)) ? signedError + 360 : signedError - 360 ) );
		error = std::abs(signedError);
		timeData.push_back(currentTime);
		errorData.push_back(signedError);
		targetData.push_back(0.0);
		currentTime += 20;

		//PID Loop
		double p = error * kp;
		integral += error;
		double i = integral * ki;
		double d = (error - lastError) * kd; //Derivative times derivative gain constant
		lastError = error;
		lastSignedError = signedError;
		double pid = p + i + d;
		pros::lcd::set_text(1, std::to_string(pid));
		PIDData.push_back(pid);
		motionProfileData.push_back(motionProfile);
		double power = motionProfile;
		if(motionProfile >= pid){ //PID triggers slowdown
			power = pid;
		}
		motorData.push_back(power);
		int direction = (signedError > 0 ? 1 : -1);
		rightWheel1.move(-power * direction); //Running in rpm values from -200 to 200.
		rightWheel2.move(-power * direction);
		rightWheel3.move(-power * direction);
		leftWheel1.move(power * direction);
		leftWheel2.move(power * direction);
		leftWheel3.move(power * direction);

		motionProfile += ( motionProfile + 25 <= 127 ? 25: 0);
		pros::delay(20);
	}
	//Resetting
	rightWheel1.move(0);
	rightWheel2.move(0);
	leftWheel1.move(0);
	leftWheel2.move(0);

	std::vector<std::pair<std::string, std::vector<float>>> data;
	data.push_back(std::make_pair("Time", timeData));
	data.push_back(std::make_pair("Error", errorData));
	data.push_back(std::make_pair("Target", targetData));
	data.push_back(std::make_pair("PID", PIDData));
	data.push_back(std::make_pair("Motion Profile", motionProfileData));
	data.push_back(std::make_pair("Motor", motorData));
	write_csv("/usd/PIDTurnData.csv", data);
}

// flywheel PID

void shoot (double targetVelocity) {
	// constants
	double kp = 0.0;
	double ki = 0.0;
	double kd = 0.0;

	double signedError = targetVelocity;
	double lastSignedError = signedError;
	double error = std::abs(signedError);
	double lastError = error;
	double integral = 0;

	int motionProfile = 0;

	float currentTime = 0;
	std::vector<float> timeData;
	std::vector<float> errorData;
	std::vector<float> targetData;
	std::vector<float> motorData;
	std::vector<float> PIDData;
	std::vector<float> motionProfileData;

	while (diskSensor.get_hue() != )
	while (error >= 0.5) {
		
		signedError = targetVelocity - flywheel.get_actual_velocity();
		error = std::abs(signedError);
		timeData.push_back(currentTime);
		errorData.push_back(signedError);
		targetData.push_back(0.0);
		currentTime += 20;

		// main PID calculations
		double p = error * kp;
		integral += error;
		double i = integral * ki;
		double d = (error - lastError) * kd;
		lastError = error;
		lastSignedError = signedError;
		double pid = p + i + d;
		PIDData.push_back(pid);
		motionProfileData.push_back(motionProfile);
		double power = motionProfile;
		if (motionProfile >= pid) {
			power = pid;
		}
		motorData.push_back(power);
		flywheel.move(power);

		motionProfile += (motionProfile + 25 <= 127 ? 25 : 0);
		pros::delay(20);
	}

	
}
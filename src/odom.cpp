#include "../include/main.h"
#include "custom.h"


// Tracking wheel distance from tracking center
double Tl = 7.25; // left
double Tr = 7.25; // right
double Tb = 7.75; // back

// Wheel travel
double dL; // left
double dR; // right
double dB; // back

double theta; // robot orientation in degrees
double xCo; // x coordinate in feet
double yCo; // y coordinate in feet

void odomCalculations() {
    uint32_t now = pros::millis();
    while(true) {
        // instantaneous angle calculation
        theta = (dL-dR) / (Tl + Tr);

        // Coordinate calculations
        yCo += 2 * ((dR / theta) + Tr) * sin(theta / 2); // y-coordinate
        xCo += 2 * ((dB / theta) + Tb) * sin(theta / 2); // x-coordinate
        
        pros::c::task_delay_until(&now, 17);
    } 
}

double getCurrentAngle() {
    return radiansToDegrees(theta); // returns angle in degrees
}

void setOdomValues(double angle, double x, double y) { //takes in angle in degrees
    theta = degreesToRadians(angle);
    xCo = x;
    yCo = y;
}
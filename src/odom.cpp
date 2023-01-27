#include "../include/main.h"
#include "initialize.hpp"
#include "pid.hpp"
#include "util.hpp"
#include <cmath>
#include <vector>

// Tracking wheel distance from tracking center
double Tl = 7.25; // left
double Tr = 7.25; // right
double Tb = 7.75; // back

// Wheel travel
double dL; // left
double dR; // right
double dB; // back

double theta; // robot orientation in degrees
double xCo;   // x coordinate in feet
double yCo;   // y coordinate in feet

void odomCalculations() {
  uint32_t now = pros::millis();
  while (true) {
    // instantaneous angle calculation
    theta = fmod(imu.get_rotation(), 360);

    // Coordinate calculations
    yCo += 2 * ((dR / theta) + Tr) * sin(theta / 2); // y-coordinate
    xCo += 2 * ((dB / theta) + Tb) *
           sin(theta / 2); // x-coordinate (cannot be performed due to no
                           // tracking wheel to measure horizontal movement)
  }
}

double getCurrentAngle() {
  return radiansToDegrees(theta); // returns angle in degrees
}

double turnToPoint(double x, double y) {
  double targetAngle = atan((y - yCo) / (x - xCo));
  turnAngle(targetAngle);
  return targetAngle;
}

void moveToPoint(double x, double y) {
  while (x != xCo && y != yCo) {
    turnToPoint(x, y);
    double hypotenuse = pow((x - xCo), 2) + pow((y - yCo), 2);
    moveDistance(hypotenuse);
  }
}

void setOdomValues(double angle, double x,
                   double y) { // takes in angle in degrees
  theta = degreesToRadians(angle);
  xCo = x;
  yCo = y;
}

// linear interpolation (lerp) and bezier curve generator for s-curves
double lerp(double p0, double p1, double t) { return p0 + (p1 - p0) * t; }

std::vector<std::vector<double>> createBezier(std::vector<double> p0,
                                              std::vector<double> p1,
                                              std::vector<double> p2,
                                              std::vector<double> p3) {

  std::vector<std::vector<double>> path;

  double t = 0;
  std::vector<double> A;
  std::vector<double> B;
  std::vector<double> C;
  std::vector<double> D;
  std::vector<double> E;
  std::vector<double> final;

  while (t != 1) {

    t += 0.01;

    // First layer
    A.push_back(lerp(p0[0], p1[0], t));
    A.push_back(lerp(p0[1], p1[1], t));
    B.push_back(lerp(p1[0], p2[0], t));
    B.push_back(lerp(p1[1], p2[1], t));
    C.push_back(lerp(p2[0], p3[0], t));
    C.push_back(lerp(p2[1], p3[1], t));

    // Second layer
    D.push_back(lerp(A[0], B[0], t));
    D.push_back(lerp(A[1], B[1], t));
    E.push_back(lerp(B[0], C[0], t));
    E.push_back(lerp(B[1], C[1], t));

    // Final point
    final[0] = lerp(D[0], E[0], t);
    final[1] = lerp(D[1], E[1], t);

    path.push_back(final);
  }

  return path;
}

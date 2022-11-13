#include "../include/main.h"

// PID / Odom variables
extern double Tl;
extern double Tr;
extern double Tb;

extern double dL;
extern double dR;
extern double dB;

extern double theta;
extern double xCo;
extern double yCo;

// PID / Odom functions
void moveDistance(double distanceInFeet);
void turnAngle(double angleInDegrees);
double getCurrentAngle();
void odomCalculations();

// S-curve functions
double lerp(double p0, double p1, double t);
std::vector<std::vector<double>> createBezier(std::vector<double> p0, std::vector<double> p1, std::vector<double> p2, std::vector<double> p3);


// Utility functions
void clearLCDLines();
float sign(float input);
float signChecker(float input, float sameSign);
float limit(float input, float max);
std::string fixFileName(std::string name);
void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<float>>> dataset);
std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename);
double radiansToDegrees(double angle);
double degreesToRadians(double angle);

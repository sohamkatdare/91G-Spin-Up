#ifndef _ODOM_H_
#define _ODOM_H_

#include <vector>

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

// S-curve functions
double lerp(double p0, double p1, double t);
std::vector<std::vector<double>> createBezier(std::vector<double> p0, std::vector<double> p1, std::vector<double> p2, std::vector<double> p3);

#endif

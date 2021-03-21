#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#define PI 3.14159265

#include "Point.h"
double distanceB2Points(Point& p1, Point& p2);
double getAngleBetweenPoints(Point& p1, Point& p2);
Point translatePoint(Point p, double distance, double angle);

#endif
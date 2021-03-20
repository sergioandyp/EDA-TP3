
#include <iostream>
#include "Geometry.h"
#include <math.h>




int distanceB2Points(Point& p1, Point& p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double getAngleBetweenPoints(Point& p1, Point& p2)
{
    double angle = atan2(p2.x - p1.x, p2.y - p1.y) * 180.0 / M_PI;
    if (angle < 0)
    {
        angle = 360 + angle;
    }
    return angle;
}

Point translatePoint(Point p, double distance, double angle)
{
    Point newPoint;
    newPoint.x = sin(angle) * distance + p.x;
    newPoint.y = cos(angle) * distance + p.y;
    return newPoint;
}

#include "Point.h"
#include <cmath>

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}

void Point::moveOffset(Point& offset) {
	this->x += offset.x;
	this->y += offset.y;
}

double Point::distanceTo(Point& point) {
	return sqrt(pow(this->x - point.x, 2) + pow(this->y - point.y, 2));
}
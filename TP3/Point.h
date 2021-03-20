#ifndef _POINT_H_
#define _POINT_H_

class Point {
public:
	double x;
	double y;

	Point(double x = 0, double y = 0);
	void moveOffset(Point& offset);
	double distanceTo(Point& point);

};

#endif
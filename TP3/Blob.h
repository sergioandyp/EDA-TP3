#ifndef _BLOB_H_
#define _BLOB_H_

#include "Point.h"

class Point {
	double x;
	double y;
};

class Blob {
	unsigned int vel;
	unsigned int angle;
	Point pos;
};

#endif
#ifndef _BLOB_H_
#define _BLOB_H_

#include "Point.h"

typedef enum { NEW_BORN, BABY_BLOB, GROWN_BLOB, GOOD_OLD_BOB } etaryGroupType;


class Blob {
public:
	unsigned int vel;
	unsigned int angle;
	unsigned int smellRadius;
	Point pos;
	etaryGroupType age;
	unsigned foodCount;
	bool isAlive;

	void move(Point& newPos);
	void moveOffset(Point& offset);
	void merge(Blob* blobToMerge);
	void feed();
};


#endif
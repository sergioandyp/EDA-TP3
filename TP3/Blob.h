#ifndef _BLOB_H_
#define _BLOB_H_

#include "Point.h"
//#include "movement.h"
//#include "World.h"

typedef enum { NEW_BORN, BABY_BLOB, GROWN_BLOB, GOOD_OLD_BLOB } etaryGroupType;
enum {BABY_FOOD = 5, GROWN_FOOD = 4, GOOD_OLD_FOOD = 3};

class Blob {
public:
	unsigned int vel;
	unsigned int angle;
	unsigned int smellRadius;
	unsigned int size;
	unsigned int ticksAlive;
	Point pos;
	etaryGroupType age;
	unsigned foodCount;
	bool isAlive;

	//void move(Point& newPos);
	void move(unsigned int percentSpeed);
	void moveOffset(Point& offset);
	//void merge(Blob* blobToMerge);
	void feed();
	bool isCollision(Blob& blob2);
};


#endif
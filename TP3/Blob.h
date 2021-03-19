#ifndef _BLOB_H_
#define _BLOB_H_

#include "Point.h"

typedef enum { NEW_BORN, BABY_BLOB, GROWN_BLOB, GOOD_OLD_BOB } etaryGroupType;



class Blob {
	unsigned int vel;
	unsigned int angle;
	Point pos;
	etaryGroupType age;
	unsigned foodCount;
	bool isAlive;
	void move(Point&)
	{

	}

};


#endif
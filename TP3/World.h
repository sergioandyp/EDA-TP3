#ifndef _WORLD_H_
#define _WORLD_H_

#include "Blob.h"
#include "Food.h"
#define MAX_BLOBS  100

enum {MODE_1, MODE_2};

class Parameters {
public:
	Parameters(unsigned int randomJiggleLimit = 360, unsigned int aliveBlobs = 1, unsigned int foodCount = 1, double maxSpeed = 10.0, double percentSpeed = 1.0,
		unsigned int smellRadius = 100, bool mode = MODE_1 );

	unsigned int foodCount;
	unsigned int randomJiggleLimit;
	double deathProb[3];
	double percentSpeed;		// Velocidad relativa de los Blobs
	double maxSpeed;			// Velocidad maxima de los Blobs
	unsigned int smellRadius;
	unsigned int aliveBlobs;	// Cantidad de blobs actualmente vivos
	bool mode;					// 0 para modo 1, 1 para modo 2
};

class WorldSize {
public:
	unsigned int babySize;
	unsigned int growSize;
	unsigned int goodOldSize;
	unsigned int foodSize;
};


class World {
public:
	Blob blobs[MAX_BLOBS];
	Food* f;
	unsigned int width;		// Tamano del mundo en pixeles
	unsigned int height;
	WorldSize sizes;
	Parameters params;
};

#endif
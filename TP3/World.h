#include "Blob.h"
#include "Food.h"
#define MAX_BLOBS  100

class Parameters {
	unsigned int foodCount;
	unsigned int randomJiggleLimit;
	double deathProb[3];
	double velRel;		// Velocidad relativa de los Blobs
	unsigned int aliveBlobs;	// Cantidad de blobs actualmente vivos
};


class World {
public:
	Blob blobs[MAX_BLOBS];
	Food* f;
	Parameters params;
};
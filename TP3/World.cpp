#include "World.h"

Parameters::Parameters(unsigned int randomJiggleLimit, unsigned int aliveBlobs, unsigned int foodCount, double maxSpeed, double percentSpeed,
		unsigned int smellRadius, bool mode) {

	this->aliveBlobs = aliveBlobs;
	this->randomJiggleLimit = randomJiggleLimit;
	this->foodCount = foodCount;
	this->maxSpeed = maxSpeed;
	this->percentSpeed = percentSpeed;
	this->smellRadius = smellRadius;
	this->deathProb[0] = 0.00005;
	this->deathProb[1] = 0.005;
	this->deathProb[2] = 0.005;
	this->mode = mode;

}
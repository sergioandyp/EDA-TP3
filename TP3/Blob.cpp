
#include <iostream>
#include "Blob.h"
#include "movement.h"
#include "Geometry.h"

void Blob::move(Point& newPos) {
	this->pos = newPos;
}
void Blob::move(unsigned int percentSpeed)
{
	this->pos = translatePoint(this->pos, this->vel * percentSpeed, this->angle);
}

void Blob::feed(void){
	this->foodCount++;
}

bool Blob::isCollision(Blob& blob2)
{
	CollBox box1(this->size, this->size, pos);
	CollBox box2(blob2.size, blob2.size, blob2.pos);
	if (box1.p.x <= box2.p.x + box2.width &&                          // Para ver si dos cajas estan chocando
		box2.p.x <= box1.p.x + box1.width &&
		box1.p.y <= box2.p.y + box2.height &&
		box2.p.y <= box1.p.y + box1.height)
		return 1;
	return 0;
}
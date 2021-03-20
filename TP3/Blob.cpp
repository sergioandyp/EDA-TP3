#include <iostream>
#include "Blob.h"

void Blob::move(Point& newPos) {
	this->pos = newPos;
}
void Blob::feed(void){
	this->foodCount++;
}

bool Blob::isCollision(Blob& blob2)
{
	CollBox box1(this->width, this->height, pos);
	CollBox box2(blob2.width, blob2.height, blob2.pos);
	if (box1.p.x <= box2.p.x + box2.width &&                          // Para ver si dos cajas estan chocando
		box2.p.x <= box1.p.x + box1.width &&
		box1.p.y <= box2.p.y + box2.height &&
		box2.p.y <= box1.p.y + box1.height)
		return 1;
	return 0;
}
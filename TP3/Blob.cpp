#include <iostream>
#include "Blob.h"

void Blob::move(Point& newPos) {
	this->pos = newPos;
}
void Blob::feed(void){
	this->foodCount++;
}
//Clase bloque

#ifndef _MOVE_H_
#define _MOVE_H_

#include "Point.h"
#include "World.h"

typedef void (*colCallback) (unsigned int* colArr, unsigned int size, void* data);


typedef unsigned int ColPair[2];

class ColReg {
public:
    ColPair* pairs;
    unsigned int size;
};


class CollBox{
public:
    CollBox(int width, int height, Point p)
    {
        this->p.x = p.x - width / 2;
        this->p.y = p.y - height / 2;    // Asumiendo sistema coordenadas allegro
        this->width = width;
        this->height = height;
    }
    Point p;
    int height;
    int width;
};


// FIX: ESTO NO DEBERIA ESTAR ACA
// Dados los parametros iniciales, crea un World con
// los 
World* createWorld(Parameters& params);

void deleteWorld(World* myWorld);

void BlobsFoodAction(World* myWorld);

void transportateBlob(World * myWorld);

void moveBlobs(World& world);

// ############ COLISIONES = 

ColReg* detectPairs(World* myWorld, etaryGroupType Age);
void checkColisions(ColReg& reg, colCallback callback, void* data);
void mergeBlobs(unsigned int* colArr, unsigned int size, World* myWorld);

#endif
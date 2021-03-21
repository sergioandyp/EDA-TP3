//Clase bloque

#ifndef _MOVE_H_
#define _MOVE_H_

#include "Point.h"
#include "World.h"

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
World* createWorld(Parameters params);

#endif
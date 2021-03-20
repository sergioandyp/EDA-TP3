//Clase bloque

#include "Point.h"

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
#ifndef _DRAW_H_
#define _DRAW_H_

#include "World.h"

int initWorld(World& world);

void destroyWorld(int error = 4);

void drawWorld(World& world);

#endif
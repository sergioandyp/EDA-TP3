#ifndef _DRAW_H_
#define _DRAW_H_

#define PRESSED 1
#define NOPRESSED 0
#define FPS 40

#include "World.h"

int initWorld(World& world);

void destroyWorld(int error = 4);

void drawWorld(World& world);

int keyboardChanges(bool estado, int tecla, World* myWorld);

void* checkEvents(bool& redraw, bool& display_close, World* myWorld);


#endif
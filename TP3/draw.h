#ifndef _DRAW_H_
#define _DRAW_H_

#define PRESSED 1
#define NOPRESSED 0

#include "World.h"

int initWorld(World& world);

void destroyWorld(int error = 4);

void drawWorld(World& world);

int keyboardChanges(bool estado, int tecla);

void checkEvents();

int parseCmdLine(int argc, char* argv[], Parameters* params, int(*pCallback) (char*, char*,Parameters*));

int parseCallback(char* key, char* value, Parameters* params);

#endif
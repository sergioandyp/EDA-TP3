#include <iostream>

#include <allegro5/allegro.h>

using namespace std;

#include "draw.h"
#include "movement.h"
#include "World.h"

#include <cstdio>


//int main(int argc, char* argv[])
//{
//	srand(time(NULL));

//	Parameters params;

//	params.aliveBlobs = 1;       
//	params.foodCount = 1;
//	params.maxSpeed = 10;
//	params.percentSpeed = 1.0;
//	params.smellRadius = 100;
//	params.deathProb[0] = 0.000005;
//	params.deathProb[1] = 0.005;
//	params.deathProb[2] = 0.005;
//	params.mode = MODE_1;
//	params.randomJiggleLimit = rand() % 360;

//	if(parseCmdLine(argc, argv, &params, parseCallback) == -1)
//	{
//		fprintf(stderr, "Input parameters are incorrect\n");
//		return 0;
//	}

//	World * myWorld = createWorld(params);

//	initWorld(*myWorld);   

//	ALLEGRO_EVENT_QUEUE* event_queue = NULL;


//	bool redraw = false;
//	bool display_close = false;
//	long ticksAllegro = 0;

//	while (!display_close) {	//Se ejecuta la animacion y el algoritmo de limpieza

//		event_queue = (ALLEGRO_EVENT_QUEUE * ) checkEvents(redraw, display_close, myWorld);

//		if (redraw && al_is_event_queue_empty(event_queue)) 
//		{
//			redraw = false;


//			// SOLO TESTEO
//			//myWorld->blobs[0].pos.x = 100;
//			//myWorld->blobs[0].pos.y = 100;

//			//myWorld->blobs[1].pos.x = 130;
//			//myWorld->blobs[1].pos.y = 100;

//			//myWorld->blobs[2].pos.x = 150;
//			//myWorld->blobs[2].pos.y = 100;

//			//myWorld->blobs[3].pos.x = 130;
//			//myWorld->blobs[3].pos.y = 100;

//			//myWorld->blobs[4].pos.x = 115;
//			//myWorld->blobs[4].pos.y = 90;
//			///

//			transportateBlob(myWorld);
//			moveBlobs(*myWorld);
//			BlobsFoodAction(myWorld);
//			makeBlobBirth(myWorld);
//			growNewBorn(myWorld);
//			blobDeath(myWorld);
//			ColReg* myColReg = detectPairs(myWorld, BABY_BLOB);		// Obtengo el registro de colisiones
//			checkColisions(*myColReg, &mergeBlobs, (void*)myWorld);		// Chequeo de colisiones multiples y BlobMerge
//			myColReg = detectPairs(myWorld, GROWN_BLOB);		// Obtengo el registro de colisiones
//			checkColisions(*myColReg, &mergeBlobs, (void*)myWorld);		// Chequeo de colisiones multiples y BlobMerge

//			free(myColReg->pairs);
//			free(myColReg);
//			drawWorld(*myWorld);
//		}
//	}
//	deleteWorld(myWorld);
//	destroyWorld();

//}
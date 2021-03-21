#include <iostream>

#include <allegro5/allegro.h>
using namespace std;

#include "draw.h"
#include "movement.h"
#include "World.h"
#include "Geometry.h"

#include <cstdio>

#define FPS 40

int main()
{

	srand(time(NULL));

	Parameters params;

	params.aliveBlobs = 10;          // TODO  randomJiggleLimit, deathProb[], mode
	params.foodCount = 0;
	params.maxSpeed = 2.5;
	params.percentSpeed = 1.0;
	params.smellRadius = 100;

	World * myWorld = createWorld(params);

	// Blob blob;
	// blob.isAlive = true;
	// blob.age = BABY_BLOB;
	// blob.vel = params.maxSpeed;
	// blob.smellRadius = params.smellRadius;
	// blob.angle = rand() % 360;
	// blob.foodCount = 0;
	// //HARDCODEADO
	// blob.size = 40;
	// blob.pos.x = 100;
	// blob.pos.y = 100;

	// Food food;
	// food.size = 20;
	// food.pos.x = 200;
	// food.pos.y = 200;

	initWorld(*myWorld);   

	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;

	timer = al_create_timer(1.0 / FPS); //Crea el timer pero NO empieza a correr
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_timer(timer);
	}

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer); 

	bool redraw = false;
	bool display_close = false;
	long ticksAllegro = 0;

	while (!display_close) {	//Se ejecuta la animacion y el algoritmo de limpieza
		ALLEGRO_EVENT ev;
		if (al_get_next_event(event_queue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
				redraw = true;

			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				display_close = true;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			transportateBlob(myWorld);
			moveBlobs(*myWorld);
			BlobsFoodAction(myWorld);
		
			// SOLO TESTEO
			//myWorld->blobs[0].pos.x = 100;
			//myWorld->blobs[0].pos.y = 100;

			//myWorld->blobs[1].pos.x = 130;
			//myWorld->blobs[1].pos.y = 100;

			//myWorld->blobs[2].pos.x = 150;
			//myWorld->blobs[2].pos.y = 100;

			//myWorld->blobs[3].pos.x = 130;
			//myWorld->blobs[3].pos.y = 100;

			//myWorld->blobs[4].pos.x = 115;
			//myWorld->blobs[4].pos.y = 90;
			///

			//drawWorld(*myWorld);

			ColReg * myColReg = detectPairs( myWorld, BABY_BLOB );		// Obtengo el registro de colisiones
			checkColisions(*myColReg, &mergeBlobs, (void*)myWorld);		// Chequeo de colisiones multiples y BlobMerge
			myColReg = detectPairs( myWorld, GROWN_BLOB );		// Obtengo el registro de colisiones
			checkColisions(*myColReg, &mergeBlobs, (void*)myWorld);		// Chequeo de colisiones multiples y BlobMerge
			
			free(myColReg->pairs);
			free(myColReg);
			drawWorld(*myWorld);
		}
	}

	deleteWorld(myWorld);

	destroyWorld();
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

}
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

	params.aliveBlobs = 1;
	params.foodCount = 5;
	params.maxSpeed = 3;
	params.percentSpeed = 1.0;
	params.smellRadius = 100;


	Blob blob;
	blob.isAlive = true;
	blob.age = BABY_BLOB;
	blob.vel = params.maxSpeed;
	blob.smellRadius = params.smellRadius;
	blob.angle = rand() % 360;
	blob.foodCount = 0;
	//HARDCODEADO
	blob.size = 40;
	blob.pos.x = 100;
	blob.pos.y = 100;

	Food food;
	food.size = 20;
	food.pos.x = 200;
	food.pos.y = 200;



	World * myWorld = createWorld(params);




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
			//myWorld->blobs[0].pos = translatePoint(myWorld->blobs[0].pos, 1, 300);
			BlobsFoodAction(myWorld);

			ColReg * myColReg = detectPairs( myWorld, GROWN_BLOB );
			checkColisions(*myColReg, colCallback callback, void* data);

			drawWorld(*myWorld);
		}
	}

	deleteWorld(myWorld);

	destroyWorld();
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

}
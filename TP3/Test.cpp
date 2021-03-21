#include <iostream>

#include <allegro5/allegro.h>
using namespace std;

#include "draw.h"
#include "movement.h"
#include "World.h"

#include <cstdio>

#define FPS 2

int main()
{
	World myWorld;

	initWorld(myWorld);

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
	al_start_timer(timer); 

	bool redraw = false;
	bool display_close = false;
	long ticksAllegro = 0;

	while (!display_close &&) {	//Se ejecuta la animacion y el algoritmo de limpieza
		ALLEGRO_EVENT ev;
		if (al_get_next_event(event_queue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
				redraw = true;

			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				display_close = true;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			myWorld.blobs[0].move(1.0);

			drawWorld(myWorld);
		}
	}

	destroyWorld();

	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

}
#include "draw.h"
#include "movement.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_color.h>    // <------- PARA PROBAR EL RADIO DE BUSQUEDA


#include <iostream>

using namespace std;

#define BACK_IMG		"background.jpg"
#define BABY_IMG		"babyblob.png"
#define GROWN_IMG		"grownblob.png"
#define GOOD_OLD_IMG	"goodoldblob.png"
#define FOOD_IMG		"food.png"
#define FONT			"OpenSans-Semibold.ttf"


#define MIN_PROB	0.0001


// punteros a elementos de allegro
static ALLEGRO_TIMER* timer;
static ALLEGRO_DISPLAY* display;
static ALLEGRO_EVENT_QUEUE* eventQueue;
static ALLEGRO_BITMAP* babyBMP;
static ALLEGRO_BITMAP* grownBMP;
static ALLEGRO_BITMAP* goodOldBMP;
static ALLEGRO_BITMAP* foodBMP;
static ALLEGRO_BITMAP* backgroundBMP;
static ALLEGRO_FONT* font20;
static ALLEGRO_FONT* font40;

static void drawBackground(unsigned int width, unsigned int height);
static void drawBlobs(Blob blobs[], unsigned int blobCount);
static void drawFood(Food food[], unsigned int foodCount);
static Point posToAll(Point pos, unsigned int width, unsigned int height);
static void drawStats(Parameters& params);


//#define TEST

#ifdef TEST

#include <ctime>

int main() {

	if (initWorld()) {
		return 1;
	}

	srand(time(NULL));

	World world;

	world.params.aliveBlobs = 10;
	world.params.foodCount = 10;

	world.f = (Food *) malloc(10 * sizeof(Food));

	for (int i = 0; i < 10; i++) {
		world.blobs[i].isAlive = 1;
		world.blobs[i].age = BABY_BLOB;
		world.blobs[i].pos = { (rand() * 900.0 / RAND_MAX), (rand() * 400.0 / RAND_MAX)};
		world.f[i].pos = { (rand() * 900.0 / RAND_MAX), (rand() * 400.0 / RAND_MAX) };
	}

	drawWorld(world);

	char x;
	cin >> x;

	destroyWorld();

	return 0;
}

#endif


void drawWorld(World& world) {

	drawBackground(world.width, world.height);

	drawFood(world.f, world.params.foodCount);

	drawBlobs(world.blobs, world.params.aliveBlobs);

	drawStats(world.params);

	al_flip_display();

}

int initWorld(World& world)
{
	int error = -1;

	if (al_init())
	{
		if (al_init_image_addon())
		{
			if (al_init_primitives_addon())
			{
				if (al_init_font_addon())
				{
					if (al_init_ttf_addon())
					{
						if (al_install_keyboard())
						{
							font40 = al_load_font(FONT, 40, 0);
							font20 = al_load_font(FONT, 20, 0);
							if (font20 && font40)
							{
								babyBMP = al_load_bitmap(BABY_IMG);
								grownBMP = al_load_bitmap(GROWN_IMG);
								foodBMP = al_load_bitmap(FOOD_IMG);
								goodOldBMP = al_load_bitmap(GOOD_OLD_IMG);
								backgroundBMP = al_load_bitmap(BACK_IMG);

								if (babyBMP && grownBMP && foodBMP && backgroundBMP && goodOldBMP)
								{
									eventQueue = al_create_event_queue();
									if (eventQueue)
									{
										display = al_create_display(world.width, world.height);
										if (display)
										{
											timer = al_create_timer(1.0 / FPS); //Crea el timer pero NO empieza a correr
											if (timer) 
											{
												
											}
											else
											{
												fprintf(stderr, "failed to create timer!\n");
												error = 5;
											}

										}
										else
										{
											fprintf(stderr, "failed to create display!\n");
											error = 4;
										}
									}
									else
									{
										fprintf(stderr, "failed to create event queue!\n");
										error = 3;
									}
								}
								else
								{
									fprintf(stderr, "failed to load bitmaps\n");
									error = 2;
								}
							}
							else
							{
								fprintf(stderr, "failed to create font!\n");
								error = 1;
							}
						}
						else
						{
							fprintf(stderr, "failed to install allegro keyboard!\n");
							error = 0;
						}
					}
					else
					{
						fprintf(stderr, "failed to initialize allegro ttf fonts!\n");
						error = 0;
					}
				}
				else
				{
					fprintf(stderr, "failed to initialize allegro fonts!\n");
					error = 0;
				}
			}
			else
			{
				fprintf(stderr, "failed to initialize allegro graphics!\n");
				error = 0;
			}
		}
		else
		{
			fprintf(stderr, "failed to initialize image addon!\n");
			error = 0;
		}
	}
	else
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		error = 0;
	}

	if (error == -1)
	{
		al_register_event_source(eventQueue, al_get_display_event_source(display));
		al_register_event_source(eventQueue, al_get_keyboard_event_source());
		al_register_event_source(eventQueue, al_get_timer_event_source(timer));
		
		al_start_timer(timer);
	}
	else
	{
		destroyWorld(error);
	}

	return error;
}

void destroyWorld(int error)
{
	switch (error)
	{
	case 4:
		if (display)
			al_destroy_display(display);
	case 3:
		if (eventQueue)
			al_destroy_event_queue(eventQueue);
	case 2:
		if (grownBMP)
			al_destroy_bitmap(grownBMP);
		if (babyBMP)
			al_destroy_bitmap(babyBMP);
		if (foodBMP)
			al_destroy_bitmap(foodBMP);
		if (goodOldBMP)
			al_destroy_bitmap(goodOldBMP);
		if (backgroundBMP)
			al_destroy_bitmap(backgroundBMP);
	case 1:
		if (font40)
			al_destroy_font(font40);
		if (font20)
			al_destroy_font(font20);
	case 0:

		break;
	}
}

// Static functions


static void drawBackground(unsigned int width, unsigned int height) {
	al_draw_scaled_bitmap(backgroundBMP, 0, 0, al_get_bitmap_width(backgroundBMP),
		al_get_bitmap_height(backgroundBMP), 0, 0, width    , height, 0);		// Imagen de fondo escalada
}

static void drawBlobs(Blob blobs[], unsigned int blobCount) {

	while (blobCount > 0) {

		if (blobs->isAlive) {
			Point pos = posToAll(blobs->pos, blobs->size, blobs->size);
			switch (blobs->age) {
				case BABY_BLOB:
				case NEW_BORN:
					al_draw_bitmap(babyBMP, pos.x, pos.y, 0);
					break;
				case GROWN_BLOB:
					al_draw_bitmap(grownBMP, pos.x, pos.y, 0);
					break;
				case GOOD_OLD_BLOB:
					al_draw_bitmap(goodOldBMP, pos.x, pos.y, 0);
					break;
				default:
					break;
			}

			blobCount--;
		}
	
		blobs++;
	}

}

static void drawFood(Food food[], unsigned int foodCount) {

	for (unsigned int i = 0; i < foodCount; i++) {
		Point pos = posToAll(food[i].pos, food[i].size, food[i].size);
		al_draw_bitmap(foodBMP, pos.x, pos.y, 0);
	}

}

static void drawStats(Parameters& params) {
	al_draw_textf(font20, al_map_rgb(0, 0, 0), 0, 0, 0, "Speed: %.2f%%", params.percentSpeed * 100.0);		//Asi separado porque tod junto se rompe
	al_draw_textf(font20, al_map_rgb(0, 0, 0), 0, 20, 0, "Food: %d", params.foodCount);
	al_draw_textf(font20, al_map_rgb(0, 0, 0), 0, 40, 0, "Smell: %d", params.smellRadius);
	al_draw_textf(font20, al_map_rgb(0, 0, 0), 0, 60, 0, "RJL: %d", params.randomJiggleLimit);
	al_draw_textf(font20, al_map_rgb(0, 0, 0), 0, 80, 0, "Death: [%.2f%%, %.2f%%, %.2f%%]", params.deathProb[0]*100.0, params.deathProb[1]*100.0, params.deathProb[2]*100.0);
}


// Dado una posicion del centro de un objeto, la transorma en
// coordenadas a la esquina superior izquierda para imprimir en Allegro
static Point posToAll(Point pos, unsigned int width, unsigned int height) {

	pos.x -= width/2.0;
	pos.y -= height/2.0;

	return pos;
}

int keyboardChanges(bool estado, int tecla, World* myWorld) {

	int salida = 0;

	if (estado == PRESSED) { // if analizando si la tecla fue presionada
	
		switch (tecla) {
		case ALLEGRO_KEY_LEFT: 
			if(myWorld->params.percentSpeed >= 0.1)
				myWorld->params.percentSpeed = myWorld->params.percentSpeed - 0.1;
			break;

		case ALLEGRO_KEY_RIGHT:
			if (myWorld->params.percentSpeed <= 0.9)
				myWorld->params.percentSpeed = myWorld->params.percentSpeed + 0.1;
			break;

		case ALLEGRO_KEY_DOWN: 
			if (myWorld->params.randomJiggleLimit >= 10)
				myWorld->params.randomJiggleLimit = myWorld->params.randomJiggleLimit - 10;
			break;

		case ALLEGRO_KEY_UP: 
			if(myWorld->params.randomJiggleLimit <=349)
				myWorld->params.randomJiggleLimit = myWorld->params.randomJiggleLimit + 10;
			break;

		case ALLEGRO_KEY_W: 
			setSmellRadius(myWorld, myWorld->params.smellRadius + 10);
			break;

		case ALLEGRO_KEY_S:
			if(myWorld->params.smellRadius >= 10)
				setSmellRadius(myWorld, myWorld->params.smellRadius - 10);
			else
				setSmellRadius(myWorld, 0);
			break;

		case ALLEGRO_KEY_A:
			if (myWorld->params.foodCount >= 1)
			{
				myWorld->params.foodCount = myWorld->params.foodCount - 1;
				setFoodCount(myWorld);
			}
			break;

		case ALLEGRO_KEY_D: 
			myWorld->params.foodCount = myWorld->params.foodCount + 1;
			setFoodCount(myWorld);
			break;

				case ALLEGRO_KEY_1:
			if (myWorld->params.deathProb[0] >= MIN_PROB)
				myWorld->params.deathProb[0] = myWorld->params.deathProb[0] - MIN_PROB;
			break;

		case ALLEGRO_KEY_2: 
			if (myWorld->params.deathProb[0] <= 1-MIN_PROB)
				myWorld->params.deathProb[0] = myWorld->params.deathProb[0] + MIN_PROB;
			break;

		case ALLEGRO_KEY_3: 
			if (myWorld->params.deathProb[1] >= MIN_PROB)
				myWorld->params.deathProb[1] = myWorld->params.deathProb[1] - MIN_PROB;
			break;

		case ALLEGRO_KEY_4: 
			if (myWorld->params.deathProb[1] <= 1-MIN_PROB)
				myWorld->params.deathProb[1] = myWorld->params.deathProb[1] + MIN_PROB;
			break;

		case ALLEGRO_KEY_5: 
			if (myWorld->params.deathProb[2] >= MIN_PROB)
				myWorld->params.deathProb[2] = myWorld->params.deathProb[2] - MIN_PROB;
			break;

		case ALLEGRO_KEY_6: 
			if (myWorld->params.deathProb[2] <= 1-MIN_PROB)
				myWorld->params.deathProb[2] = myWorld->params.deathProb[2] + MIN_PROB;
			break;
		}
	}
	return salida;
}

void* checkEvents(bool& redraw, bool& display_close, World* myWorld)
{
	ALLEGRO_EVENT event;

	if (al_get_next_event(eventQueue, &event))
	{
		switch (event.type) {                    //Se evalua el evento ocurrido y se actua acordemente
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			display_close = true;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			keyboardChanges(PRESSED, event.keyboard.keycode, myWorld);
			break;

		case ALLEGRO_EVENT_KEY_UP:
			keyboardChanges(NOPRESSED, event.keyboard.keycode, myWorld);
			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			break;

		case ALLEGRO_EVENT_TIMER:
			redraw = true;
			break;

		}
	}
	return eventQueue;
}
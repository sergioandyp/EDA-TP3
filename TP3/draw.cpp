#include "draw.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <iostream>

//#define SCALE_FACTOR	1.0		// Factor por el que escalar la pantalla donde 1.0 es el tamano original del background

//#define DISP_WIDTH	(SCALE_FACTOR*al_get_bitmap_width(backgroundBMP))
//#define DISP_HEIGHT	(SCALE_FACTOR*al_get_bitmap_height(backgroundBMP))

using namespace std;

#define BACK_IMG		"background.jpg"
#define BABY_IMG		"babyblob.png"
#define GROWN_IMG		"grownblob.png"
#define GOOD_OLD_IMG	"goodoldblob.png"
#define FOOD_IMG		"food.png"

// punteros a elementos de allegro
ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* eventQueue;
ALLEGRO_BITMAP* babyBMP;
ALLEGRO_BITMAP* grownBMP;
ALLEGRO_BITMAP* goodOldBMP;
ALLEGRO_BITMAP* foodBMP;
ALLEGRO_BITMAP* backgroundBMP;
ALLEGRO_FONT* font20;
ALLEGRO_FONT* font40;

static void drawBackground(unsigned int width, unsigned int height);
static void drawBlobs(Blob blobs[], unsigned int blobCount);
static void drawFood(Food food[], unsigned int foodCount);

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
							//font40 = al_load_font("./maiddroid_files/assets/OpenSans-Semibold.ttf", 40, 0);
							//font20 = al_load_font("./maiddroid_files/assets/OpenSans-Semibold.ttf", 20, 0);
							//if (font20 && font40)
							//{
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
							//}
							//else
							//{
							//	fprintf(stderr, "failed to create font!\n");
							//	error = 1;
							//}
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
			switch (blobs->age) {
				case BABY_BLOB:
					al_draw_bitmap(babyBMP, blobs->pos.x, blobs->pos.y, 0);
					break;
				case GROWN_BLOB:
					al_draw_bitmap(grownBMP, blobs->pos.x, blobs->pos.y, 0);
					break;
				case GOOD_OLD_BLOB:
					al_draw_bitmap(goodOldBMP, blobs->pos.x, blobs->pos.y, 0);
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
		al_draw_bitmap(foodBMP, food[i].pos.x, food[i].pos.y, 0);
	}

}
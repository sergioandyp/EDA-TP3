#include "draw.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <iostream>

#define SCALE_FACTOR	1.0		// Factor por el que escalar la pantalla donde 1.0 es el tamano original del background

#define DISP_WIDTH	(SCALE_FACTOR*al_get_bitmap_width(backgroundBMP))
#define DISP_HEIGHT	(SCALE_FACTOR*al_get_bitmap_height(backgroundBMP))

using namespace std;

#define BACK_IMG		"background.jpg"
#define BABY_IMG		"babyblob.png"
#define GROWN_IMG		"grownblob.png"
#define GOOD_OLD_IMG	"goodoldblob.png"
#define FOOD_IMG		"food.png"

ALLEGRO_DISPLAY* display = NULL;

// Bitmap de los sprites
ALLEGRO_BITMAP* backgroundBMP = NULL;
ALLEGRO_BITMAP* babyBMP = NULL;
ALLEGRO_BITMAP* grownBMP = NULL;
ALLEGRO_BITMAP* goodOldBMP = NULL;
ALLEGRO_BITMAP* foodBMP = NULL;

static void drawBackground();
static void drawBlobs(Blob blobs[], unsigned int blobCount);
static void drawFood(Food food[], unsigned int foodCount);

#define TEST

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

	drawBackground();

	drawFood(world.f, world.params.foodCount);

	drawBlobs(world.blobs, world.params.aliveBlobs);

	al_flip_display();

}

int initWorld() {

	if (!al_init()) {
		cout << "Error al inicializar Allegro";
		return 1;
	}

	if (!al_init_image_addon()) {
		cout << "Error al inicializar image addon";
		return 1;
	}

	backgroundBMP = al_load_bitmap(BACK_IMG);
	if (!backgroundBMP) {
		cout << "Error al inicializar el backgroundBMP";
		return 1;
	}

	babyBMP = al_load_bitmap(BABY_IMG);
	if (!babyBMP) {
		cout << "Error al inicializar el babyBMP";
		return 1;
	}

	grownBMP = al_load_bitmap(GROWN_IMG);
	if (!grownBMP) {
		cout << "Error al inicializar el grownBMP";
		return 1;
	}

	goodOldBMP = al_load_bitmap(GOOD_OLD_IMG);
	if (!goodOldBMP) {
		cout << "Error al inicializar el goodOldBMP";
		return 1;
	}

	foodBMP = al_load_bitmap(FOOD_IMG);
	if (!foodBMP) {
		cout << "Error al inicializar el foodBMP";
		return 1;
	}

	display = al_create_display(DISP_WIDTH, DISP_HEIGHT);
	if (!display) {
		cout << "Error al inicializar el display";
		return 1;
	}

	return 0;
}

void destroyWorld() {
	al_destroy_bitmap(backgroundBMP);
	al_destroy_bitmap(babyBMP);
	al_destroy_bitmap(grownBMP);
	al_destroy_bitmap(goodOldBMP);
	al_destroy_display(display);
	al_shutdown_image_addon();
}


// Static functions


static void drawBackground() {
	al_draw_scaled_bitmap(backgroundBMP, 0, 0, al_get_bitmap_width(backgroundBMP),
		al_get_bitmap_height(backgroundBMP), 0, 0, DISP_WIDTH, DISP_HEIGHT, 0);		// Imagen de fondo escalada
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
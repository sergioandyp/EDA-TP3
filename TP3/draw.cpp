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

static void drawBackground();
static void drawBlobs();

int main() {

	if (initWorld()) {
		return 1;
	}

	drawWorld();

	char x;
	cin >> x;

	destroyWorld();

	return 0;
}

void drawWorld() {

	drawBackground();

	drawBlobs();

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

static void drawBlobs() {

	al_draw_bitmap(babyBMP, 100, 100, 0);
	al_draw_bitmap(grownBMP, 200, 200, 0);
	al_draw_bitmap(goodOldBMP, 300, 300, 0);

}
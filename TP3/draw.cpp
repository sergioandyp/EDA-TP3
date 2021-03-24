#include "draw.h"

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
#define ERROR			-1
#define NOPARAM			0
#define OK				1

// punteros a elementos de allegro
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

char keys[8][20] = {
					"mode",
					"numberOfBlobs",
					"maxVelBlobs",
					"%VelBlobs",
					"smellRadius",
					"randomJiggleLimit",
					"deathChance",
					"foodCount"
};


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

	//DEBUG
	int i = 0;

	while (blobCount > 0) {

		if (blobs->isAlive) {
			Point pos = posToAll(blobs->pos, blobs->size, blobs->size);
			switch (blobs->age) {
				case BABY_BLOB:
				case NEW_BORN:
					al_draw_bitmap(babyBMP, pos.x, pos.y, 0);
					//al_draw_circle(blobs->pos.x, blobs->pos.y, blobs->smellRadius, al_color_name("red"), 2.0);
					al_draw_textf(font40, al_map_rgb(0,0,0), pos.x, pos.y, 0, "%d", i);	// DEBUG
					break;
				case GROWN_BLOB:
					al_draw_bitmap(grownBMP, pos.x, pos.y, 0);
					//al_draw_circle(blobs->pos.x, blobs->pos.y, blobs->smellRadius, al_color_name("red"), 2.0);
					al_draw_textf(font40, al_map_rgb(0,0,0), pos.x, pos.y, 0, "%d", i);	// DEBUG
					break;
				case GOOD_OLD_BLOB:
					al_draw_bitmap(goodOldBMP, pos.x, pos.y, 0);
					//al_draw_circle(blobs->pos.x, blobs->pos.y, blobs->smellRadius, al_color_name("red"), 2.0);
					al_draw_textf(font40, al_map_rgb(0,0,0), pos.x, pos.y, 0, "%d", i);	// DEBUG
					break;
				default:
					break;
			}
			blobCount--;
		}
	
		blobs++;
		// DEBUG
		i++;
	}

}

static void drawFood(Food food[], unsigned int foodCount) {

	for (unsigned int i = 0; i < foodCount; i++) {
		Point pos = posToAll(food[i].pos, food[i].size, food[i].size);
		al_draw_bitmap(foodBMP, pos.x, pos.y, 0);
	}

}

// Dado una posicion del centro de un objeto, la transorma en
// coordenadas a la esquina superior izquierda para imprimir en Allegro
static Point posToAll(Point pos, unsigned int width, unsigned int height) {

	pos.x -= width/2.0;
	pos.y -= height/2.0;

	return pos;

}

int parseCmdLine(int argc, char* argv[], Parameters* params, int(*pCallback) (char*, char*, Parameters*)) {//cant de opciones +params o -1 en error
    int i = 1;//la primer palabra,con indice 0, es el nombre del programa, y esta no se cuenta
    int datos = 0;//params+opciones
    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {//si hay opcion
            if (i != (argc - 1)) {//si no es el ultimo elemento que recibe
                if (argv[i][1] == '\0') {//si es una clave sin valor
                    return ERROR;
                }
                else {
                    if (pCallback(argv[i]+1, argv[i + 1], params) == 1) {//si esta todo bien..
                        ++datos;
                        ++i;//salteo el valor de la clave
                    }
                    else {//parsecallback tiro error
                        return ERROR;
                    }
                }
            }
            else {
                return ERROR;
            }
        }
        else {//encontre un parametro
            if (pCallback(NULL, argv[i], params) == 1) {//si esta todo bien
                ++datos;
            }
            else {//parsecallback tiro error
                return ERROR;
            }
        }
    }
    return datos;
}
////////////////////////////////////////////////////////////
int parseCallback(char* key, char* value, Parameters* params) {//0 si no es valido 1 si si
	int i = 0;
    if (key == NULL) {//es un parametro
		
        return NOPARAM;
    }
    else {//es una opcion
        if (key[1] == NULL) {//clave vacia devuelve error
            return NOPARAM;
        }
        else {
            if (value == NULL) {//si el valor es null es una clave sin valor y devuelve error
                return NOPARAM;
            }
            else {
				for (i = 0; i <= 7; i++)
				{
					if (strcmp(keys[i], key) == 0)
						break;
				}
				if (i == 8)
					return NOPARAM;
				switch (i)
				{
					case 0:
						if (value[0] == '1')
							params->mode = 1;
						else
						{
							if (value[0] == '2')
								params->mode = 2;
							else
								return NOPARAM;
						}
						break;
					case 1:
						if (value[0] >= '1' && value[0] <= '9')
						{
							params->aliveBlobs = atoi(value);
						}
						else
							return NOPARAM;
						break;
					case 2:
						if (value[0] >= '0' && value[0] <= '9')
						{
							params->maxSpeed = atoi(value); // o sino strtod (value)
						}
						else
							return NOPARAM;
						break;
					case 3:
						if (value[0] >= '0' && value[0] <= '9')
						{
							params->percentSpeed = atoi(value); // o sino strtod (value)
						}
						else
							return NOPARAM;
						break;
					case 4:
						if (value[0] >= '0' && value[0] <= '9')
						{
							params->smellRadius = atoi(value); // o sino strtod (value)
						}
						else
							return NOPARAM;
						break;
					case 5:
						if (value[0] >= '0' && value[0] <= '9' && atoi(value) <= 360)
						{
							params->randomJiggleLimit = atoi(value);
						}
						else
							return NOPARAM;
						break;
					case 6:
						if (value[0] >= '0' && value[0] <= '9')
						{
							params->deathProb[0] = strtod(value,NULL);
							params->deathProb[1] = strtod(value, NULL);
							params->deathProb[2] = strtod(value, NULL);
						}
							return NOPARAM;
						break;
					case 7:
						if (value[0] >= '0' && value[0] <= '9')
						{
							params->foodCount = atoi(value);
						}
						else
							return NOPARAM;
						break;
				}

                return OK;
            }
        }
         
    }
}

#include "Events.h"

int keyboardChanges(bool estado, int tecla) {

	int salida = 0;

	if (estado == PRESSED) { // if analizando si la tecla fue presionada
		switch (tecla) {
		case ALLEGRO_KEY_0: 

			break;

		case ALLEGRO_KEY_1: 

			break;

		case ALLEGRO_KEY_2: 

			break;

		case ALLEGRO_KEY_3: 

			break;

		case ALLEGRO_KEY_4: 

			break;

		case ALLEGRO_KEY_5: 

			break;

		case ALLEGRO_KEY_6: 

			break;

		case ALLEGRO_KEY_7: 

			break;

		case ALLEGRO_KEY_Q: 

			break;

		case ALLEGRO_KEY_E: 

			break;

		case ALLEGRO_KEY_P: 

			break;

		case ALLEGRO_KEY_C: 

			break;

		case ALLEGRO_KEY_I: 

			break;

		case ALLEGRO_KEY_A: 

			break;

		case ALLEGRO_KEY_B: 

			break;
		}
	}
	return salida;
}

void checkEvents()
{
	ALLEGRO_EVENT evento;
	int do_exit = 0;

	al_wait_for_event(eventQueue, &evento); //Espera que ocurra un evento

	switch (evento.type) {                    //Se evalua el evento ocurrido y se actua acordemente
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			do_exit = 1;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			keyboardChanges(PRESSED, evento.keyboard.keycode);
			break;

		case ALLEGRO_EVENT_KEY_UP:
			keyboardChanges(NOPRESSED, evento.keyboard.keycode);
			break;

		case ALLEGRO_EVENT_TIMER:
			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			break;
	}
}
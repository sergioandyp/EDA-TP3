#include <iostream>
#include <cstdio>

#include <allegro5/allegro.h>


#include "draw.h"
#include "movement.h"
#include "World.h"
#include "parse.h"

using namespace std;

int parserCallback(char* key, char* value, void* parameters); //0 si no es valido 1 si si

const char keys[10][20] = {
					"mode",
					"numberOfBlobs",
					"maxVelBlobs",
					"%VelBlobs",
					"smellRadius",
					"randomJiggleLimit",
					"deathChance1",
					"deathChance2",
					"deathChance3",
					"foodCount"
};

int main(int argc, char* argv[])
{
	srand(time(NULL));

	Parameters params;

	if (parseCmdLine(argc, argv, &params, parserCallback) == -1)
	{
		fprintf(stderr, "Input parameters are incorrect\n");
		return 0;
	}

	World* myWorld = createWorld(params);

	initWorld(*myWorld);

	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	bool redraw = false;
	bool display_close = false;
	long ticksAllegro = 0;

	while (!display_close) {	//Se ejecuta la animacion y el algoritmo de limpieza

		event_queue = (ALLEGRO_EVENT_QUEUE*)checkEvents(redraw, display_close, myWorld);

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			transportateBlob(myWorld);
			moveBlobs(*myWorld);
			BlobsFoodAction(myWorld);
			makeBlobBirth(myWorld);
			growNewBorn(myWorld);
			blobDeath(myWorld);
			ColReg* myColReg = detectPairs(myWorld, BABY_BLOB);		// Obtengo el registro de colisiones
			checkColisions(*myColReg, &mergeBlobs, (void*)myWorld);		// Chequeo de colisiones multiples y BlobMerge
			myColReg = detectPairs(myWorld, GROWN_BLOB);		// Ahora lo mismo para los GROWN
			checkColisions(*myColReg, &mergeBlobs, (void*)myWorld);

			drawWorld(*myWorld);
			free(myColReg->pairs);
			free(myColReg);
		}
	}
	deleteWorld(myWorld);
	destroyWorld();
}

int parserCallback(char* key, char* value, void* parameters) {//0 si no es valido 1 si si
	
	Parameters* params = (Parameters*)parameters;

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
				for (i = 0; i <= 9; i++)
				{
					if (strcmp(keys[i], key) == 0)
						break;
				}
				if (i == 10)
					return NOPARAM;
				switch (i)
				{
					case 0:
						if (value[0] == '1')
							params->mode = MODE_1;
						else
						{
							if (value[0] == '2')
								params->mode = MODE_2;
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
							params->percentSpeed = strtod(value,NULL); // o sino atoi (value) /100
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
						}
							return NOPARAM;
						break;
					case 7:
						if (value[0] >= '0' && value[0] <= '9')
						{
							params->deathProb[1] = strtod(value, NULL);
						}
							return NOPARAM;
						break;
					case 8:
						if (value[0] >= '0' && value[0] <= '9')
						{
							params->deathProb[2] = strtod(value, NULL);
						}
							return NOPARAM;
						break;
					case 9:
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
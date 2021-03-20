/*#include "Allegro.h"

int init_allegro(allegroAssets * allegroPointers)
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
						allegroPointers->font40 = al_load_font("./maiddroid_files/assets/OpenSans-Semibold.ttf", 40, 0);
						allegroPointers->font20 = al_load_font("./maiddroid_files/assets/OpenSans-Semibold.ttf", 20, 0);
						if (allegroPointers->font20 && allegroPointers->font40)
						{
							allegroPointers->smallBlob = al_load_bitmap("./maiddroid_files/assets/smallBlob.png");
							allegroPointers->bigBlob = al_load_bitmap("./maiddroid_files/assets/bigBlob.png");
							allegroPointers->food = al_load_bitmap("./maiddroid_files/assets/food.png");
							allegroPointers->background = al_load_bitmap("./maiddroid_files/assets/background.png");
							if (allegroPointers->smallBlob && allegroPointers->bigBlob && allegroPointers->food && allegroPointers->background)
							{
								allegroPointers->eventQueue = al_create_event_queue();
								if (allegroPointers->eventQueue)
								{
									allegroPointers->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
									if (allegroPointers->display)
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
		
	if (error = -1)
	{
		al_register_event_source(allegroPointers->eventQueue, al_get_display_event_source(allegroPointers->display));
		al_register_event_source(allegroPointers->eventQueue, al_get_keyboard_event_source());
	}
	else
	{
		destroyAllegroAssets(allegroPointers, error);
	}

	return error;
}

void destroyAllegroAssets(allegroAssets* allegroPointers, int error = 4)
{
	switch (error)
	{
		case 4:
			if (allegroPointers->display)
				al_destroy_display(allegroPointers->display);
		case 3:
			if (allegroPointers->eventQueue)
				al_destroy_event_queue(allegroPointers->eventQueue);
		case 2:
			if (allegroPointers->bigBlob)
				al_destroy_bitmap(allegroPointers->bigBlob);
			if (allegroPointers->smallBlob)
				al_destroy_bitmap(allegroPointers->smallBlob);
			if (allegroPointers->food)
				al_destroy_bitmap(allegroPointers->food);
			if (allegroPointers->background)
				al_destroy_bitmap(allegroPointers->background);
		case 1:
			if(allegroPointers->font40)
				al_destroy_font(allegroPointers->font40);
			if (allegroPointers->font20)
				al_destroy_font(allegroPointers->font20);
		case 0:

			break;
	}
}*/
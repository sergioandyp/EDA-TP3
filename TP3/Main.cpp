#include "draw.h"
#include <allegro5/allegro.h>
#include "Events.h"



int main()
{
    ALLEGRO_EVENT evento;
    int do_exit=0;

	al_wait_for_event(eventQueue, &evento); //Espera que ocurra un evento
        
        switch(evento.type){                    //Se evalua el evento ocurrido y se actua acordemente
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                do_exit = 1;
                break;
                
            case ALLEGRO_EVENT_KEY_DOWN:
                keyboardChanges (PRESSED, evento.keyboard.keycode);
                break;
                
            case ALLEGRO_EVENT_KEY_UP:   
                keyboardChanges (NOPRESSED, evento.keyboard.keycode);
                break;
                
            case ALLEGRO_EVENT_TIMER:
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                break;
                
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                break;
        }
}
#include "Events.h"

int keyboardChanges(bool estado, int tecla) {

    int salida = 0;

    if (estado == PRESSED) { // if analizando si la tecla fue presionada
        switch (tecla) {
        case ALLEGRO_KEY_0: // se presiona la tecla 0 y se cambia el estado del b0
            
            break;

        case ALLEGRO_KEY_1: // se presiona la tecla 1 y se cambia el estado del b1
            
            break;

        case ALLEGRO_KEY_2: // se presiona la tecla 2 y se cambia el estado del b2
            
            break;

        case ALLEGRO_KEY_3: // se presiona la tecla 3 y se cambia el estado del b3
            
            break;

        case ALLEGRO_KEY_4: // se presiona la tecla 4 y se cambia el estado del b4
            
            break;

        case ALLEGRO_KEY_5: // se presiona la tecla 5 y se cambia el estado del b5
           
            break;

        case ALLEGRO_KEY_6: // se presiona la tecla 6 y se cambia el estado del b6
            
            break;

        case ALLEGRO_KEY_7: // se presiona la tecla 7 y se cambia el estado del b7
            
            break;

        case ALLEGRO_KEY_Q: // se presiona la tecla Q y se avisa que se debe cerrar el programa
            
            break;

        case ALLEGRO_KEY_E: // se presiona la tecla 0 y se encienden todos los bits del puerto
           
            break;

        case ALLEGRO_KEY_P: // se presiona la tecla 0 y se avisa que se debe comenzar a parpadear
           
            break;

        case ALLEGRO_KEY_C: // se presiona la tecla 0 y se apagan todos los bits del puerto
            
            break;

        case ALLEGRO_KEY_I: // se presiona la tecla 0 y se invierten todos los bits del puerto
            
            break;

        case ALLEGRO_KEY_A: // se presiona la tecla 0 y se activa el puerto A
            
            break;

        case ALLEGRO_KEY_B: // se presiona la tecla 0 y se activa el puerto B
            
            break;
        }
    }
    return salida;
}
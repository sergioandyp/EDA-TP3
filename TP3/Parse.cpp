#include <iostream>

#include "parse.h"

int parseCmdLine(int argc, char* argv[], void* params, parseCallback pCallback) {//cant de opciones +params o -1 en error
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
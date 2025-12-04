#ifndef INTERPRETE_H
#define INTERPRETE_H

#include "006.h" // debe conocer el Diccionario

class Interprete {
    private:
        DiccionarioCuacs diccionario;
    public:

        Interprete(); // Constructor sin argumentos
        void ejecutarCuacker(); // Método principal (ejecuta toda la lógica)
};



#endif
#ifndef DICCIONARIOCUACS_H
#define DICCIONARIOCUACS_H

#include "Cuac.h"
#include "TablaHash.h"
#include "ArbolAVL.h"

#include<string>
using namespace std;

// Implementación recomendada por la presentación

class DiccionarioCuacs {

    private:
        TablaHash tabla;
        Arbol_AVL arbol;

    public:

        DiccionarioCuacs(); // Declaración del Constructor

        // Métodos
        void insertar(Cuac &nuevo);
        void last (int N);
        void follow (string nombre);
        void date(Fecha fecha, Fecha fecha2);

        int numElem(){
            return tabla.nElem();
        }
};

#endif

#ifndef MODULO_006_H
#define MODULO_006_H

#include "004.h"
#include "200.h"
#include "300.h"

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

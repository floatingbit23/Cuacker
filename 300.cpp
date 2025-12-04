#include "300.h"
#include <list>
#include<iostream>

// constructor
Arbol_AVL::Arbol_AVL() {
    raiz = nullptr;
}
// destructor
Arbol_AVL::~Arbol_AVL() {
    delete raiz;
}

//getters
int Arbol_AVL::get_Altura(Nodo* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    return nodo->altura;
}

int Arbol_AVL::get_Balanceo(Nodo* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    int balanceo = (get_Altura(nodo->hijoI) - get_Altura(nodo->hijoD));
    return balanceo;
}

// funnciones
Nodo* Arbol_AVL::giro_Derecha(Nodo* nodoD) {
    Nodo* nodoHI = nodoD->hijoI; //nodoHI = hijo iizquierdo del nodoD
    Nodo* nodoHD = nodoHI->hijoD; //nodoHD = hijo derecho del nodoHI

    nodoHI->hijoD = nodoD;
    nodoD->hijoI = nodoHD;
    nodoD->altura = max(get_Altura(nodoD->hijoI), get_Altura(nodoD->hijoD)) + 1; //altura del hijo maxima entre las 2 ramas + 1(el nodo actual)
    nodoHI->altura = max(get_Altura(nodoHI->hijoI), get_Altura(nodoHI->hijoD)) + 1;
    return nodoHI; // nueva raiz del arbol
}

Nodo* Arbol_AVL::giro_Izquierda(Nodo* nodoI) {
    Nodo* nodoHD = nodoI->hijoD; //nodoHD = hijo derecho del nodoI
    Nodo* nodoHI = nodoHD->hijoI; //nodoHI = hijo izquierddo del nodoHD

    nodoHD->hijoI = nodoI;
    nodoI->hijoD = nodoHI;
    nodoI->altura = max(get_Altura(nodoI->hijoI), get_Altura(nodoI->hijoD)) + 1; //altura del hijo maxima entre las 2 ramas + 1(el nodo actual)
    nodoHD->altura = max(get_Altura(nodoHD->hijoI), get_Altura(nodoHD->hijoD)) + 1;
    return nodoHD; // nueva raiz del arbol
}

/*
        N
      /   \
(Fmenor) (F2mayor)

*/

//cambiado de nodos a usar listas
//E elgido la opcion de listas de punteros debido a que
//mi annterior implementacion conn nodos no era optima
//y exxcediaa el limite de tiempo
Nodo* Arbol_AVL::insertar(Nodo* nodo, Cuac* clave) {
    if (nodo == nullptr) { //caso base (como maude)
        return new Nodo(clave);
    }
    Fecha fechaC = clave->get_fecha();
    if (fechaC.es_menor(nodo->fecha)) {
        nodo->hijoI = insertar(nodo->hijoI, clave); // izq
    } else if (nodo->fecha.es_menor(fechaC)) {
        nodo->hijoD = insertar(nodo->hijoD, clave); // der
    } else {
        bool insertado = false;
        for (list<Cuac*>::iterator itera_cuac = nodo->lcuac.begin(); itera_cuac != nodo->lcuac.end(); ++itera_cuac) {
            Cuac* EA = *itera_cuac;
            if (clave->get_texto() < EA->get_texto()) {
                nodo->lcuac.insert(itera_cuac, clave);
                insertado = true;
                break;
            } else if (clave->get_texto() == EA->get_texto()) {
                if (clave->get_usuario() < EA->get_usuario()) {
                    nodo->lcuac.insert(itera_cuac,clave);
                    insertado = true;
                    break;
                }
            }
        }
        if (insertado == false) {
            nodo->lcuac.push_back(clave);
        }

        return nodo; //empate -> no cambbai altura
    }

    // nuevo nodo, acctualziamso alturra
    nodo->altura = max(get_Altura(nodo->hijoI), get_Altura(nodo->hijoD)) + 1;
    // RII
    if ((get_Balanceo(nodo) > 1) && (fechaC.es_menor(nodo->hijoI->fecha))) {
        return giro_Derecha(nodo);
    }
    // RDD
    if ((get_Balanceo(nodo) < -1) && (nodo->hijoD->fecha.es_menor(fechaC))) {
        return giro_Izquierda(nodo);
    }
    // RDI
    if ((get_Balanceo(nodo) > 1) && (nodo->hijoI->fecha.es_menor(fechaC))) {
        nodo->hijoI = giro_Izquierda(nodo->hijoI);
        return giro_Derecha(nodo);
    }
    // RID
    if ((get_Balanceo(nodo) < -1) && (fechaC.es_menor(nodo->hijoD->fecha))) {
        nodo->hijoD = giro_Derecha(nodo->hijoD);
        return giro_Izquierda(nodo);
    }
    return nodo;

}

void Arbol_AVL::last(Nodo* nodo, int& restantes, int& contador) {
    if ((nodo == nullptr) || (restantes <= 0)) {
        return;
    }
    last(nodo->hijoD, restantes, contador); //colocamos a la derecha los mas recientes
    if (restantes > 0) {
        for (Cuac* c : nodo->lcuac) {
            if (restantes <= 0) {
                break; //fin
            }
            cout << contador << ". ";
            c->write_cuac();
            cout << "\n";
            contador = contador + 1;
            restantes = restantes - 1;
        }
    }

    if (restantes > 0) {
        last(nodo->hijoI, restantes, contador); //colocamos a la izuierda los mas antiguos
    }
}

void Arbol_AVL::date(Nodo* nodo, Fecha fecha, Fecha fecha2, int& contador) {
    if (nodo == nullptr) {
        return;
    }
    if ((nodo->fecha.es_menor(fecha2)) || (nodo->fecha.es_igual(fecha2))) {
        // nodo <= fecha2
        date(nodo->hijoD, fecha, fecha2, contador); //derecha
    }
    if ((!nodo->fecha.es_menor(fecha) == true) && (!fecha2.es_menor(nodo->fecha) == true)) {
        for (Cuac* c : nodo->lcuac) {
            contador++;
            cout << contador << ". ";
            c->write_cuac();
            cout << "\n";
        }
    }

    if ((fecha.es_menor(nodo->fecha)) || (fecha.es_igual(nodo->fecha))) {
        // nodo >= 1
        date(nodo->hijoI, fecha, fecha2, contador); //izquierda
    }
}

//llamada a la funcion
void Arbol_AVL::insertar(Cuac* c) {
    raiz = insertar(raiz, c);
}

void Arbol_AVL::date(Fecha fecha, Fecha fecha2) {
    int contador = 0;
    date(raiz, fecha, fecha2, contador);
    cout << "Total: " << contador << " cuac" << "\n";

}

void Arbol_AVL::last(int n) {
    int cuacs, restantes, contador;
    restantes = n;
    contador = 1;
    last(raiz, restantes, contador);
    cuacs = n - max(0, restantes);
    cout << "Total: " << cuacs << " cuac" << endl;
}

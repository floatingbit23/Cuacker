#ifndef ARBOLAVL_H
#define ARBOLAVL_H
#include <list>
#include "Cuac.h"

//Arbol AVL
class Nodo {
    friend class Arbol_AVL;

private:
    Nodo *hijoI, *hijoD;
    int altura;
    std::list<Cuac*> lcuac;
    Fecha fecha;
    bool ordenada;

public:
    Nodo(Cuac* c) {
    ordenada = true;
    fecha = c->get_fecha();
    lcuac.push_back(c);
    hijoI = nullptr;
    hijoD = nullptr;
    altura = 1; // definno la variable altura ddirectamente en la variable nodo para evitar calcularla en funcion O(1) en veez de O(n)
    }

    ~Nodo() { // destructor pdf
        delete hijoI;
        delete hijoD;
    }
};

class Arbol_AVL {
private:
    Nodo* raiz;

    //getters
    int get_Altura(Nodo* nodo);
    int get_Balanceo(Nodo* nodo);

    //giros
    Nodo* giro_Derecha(Nodo* nodoD);
    Nodo* giro_Izquierda(Nodo* nodoI);
    Nodo* insertar(Nodo* nodo, Cuac* clave);

    void last(Nodo* nodo, int N);
    void date(Nodo* nodo, Fecha fecha, Fecha fecha2, int& contador);
    void last(Nodo* nodo, int& restantes, int& contador);
public:
    Arbol_AVL();
    ~Arbol_AVL();
    void insertar(Cuac* c);
    void date(Fecha fecha, Fecha fecha2);
    void last(int n);
};

#endif // 300_H_INCLUDED

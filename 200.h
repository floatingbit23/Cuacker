#ifndef MODULO_200_H
#define MODULO_200_H

#include "004.h"
#include <iostream>
#include<string>
#include<list>

//opcion 2 de alamacenamiento
class TablaHash
{
private:
    class Par {
        string nombre;
        list<Cuac> l;
        friend class TablaHash;
    };
    list<Par>* T;
    int B;
    int n;

public:
    TablaHash();
    ~TablaHash();
    int funcionHash(string& clave);
    Cuac* insertar(Cuac c); //modificado para que devuelva puntero en vez de void
    list<Cuac> follow(string& usuario);
    int nElem(void) {
        return n;
        }
};

#endif // 200_H_INCLUDED

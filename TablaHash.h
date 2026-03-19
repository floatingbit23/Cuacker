#ifndef TABLAHASH_H
#define TABLAHASH_H

#include "Cuac.h"
#include<string>
#include<list>

//opcion 2 de almacenamiento
class TablaHash
{
private:
    class Par {
        std::string nombre;
        std::list<Cuac> l;
        friend class TablaHash;
    };
    std::list<Par>* T;
    int B;
    int n;

public:
    TablaHash();
    ~TablaHash();
    int funcionHash(std::string& clave);
    Cuac* insertar(Cuac c); //modificado para que devuelva puntero en vez de void
    std::list<Cuac> follow(std::string& usuario);
    int nElem(void) {
        return n;
        }
};

#endif // 200_H_INCLUDED

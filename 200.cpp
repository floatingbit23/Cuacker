#include <iostream>
#include <list>
#include <string>
#include "003.h"
#include "004.h"
#include "200.h"

using namespace std;

// constructor:
TablaHash::TablaHash() {
    B = 300007;
    T = new list<Par>[B]; // B = M
    n = 0; //n = nElem
}

// Destructor (memoria dinamica)
TablaHash::~TablaHash() {
    delete[] T;
}

int TablaHash::funcionHash(string& clave)
{

    unsigned long suma = 0;
    for (unsigned char c : clave) {
        suma = (suma * 31) + c;
    }
    return suma % B;
}

Cuac* TablaHash::insertar(Cuac c) // modificado para delvolver punntero en vez de void
{
    string usuario = c.get_usuario();
    int h = funcionHash(usuario);


    for (Par &par : T[h]) {
        if (par.nombre == usuario) {
            Fecha fecha = c.get_fecha();
            string texto = c.get_texto();

            list<Cuac>::iterator itera_Cuac = par.l.begin();

            while (itera_Cuac != par.l.end()) {

                Fecha fecha2 = itera_Cuac->get_fecha();



                if (fecha.es_menor(fecha2)) {
                    ++itera_Cuac;
                    continue;
                } else if (fecha.es_igual(fecha2)){
                    //ordenamos por texto
                    string texto2 = itera_Cuac->get_texto();
                    if (texto > texto2) {
                        ++itera_Cuac;
                        continue;
                    } else if (texto == texto2) {
                        //orrdenamos por usuario
                        string usuario2 = itera_Cuac->get_usuario();
                        if (usuario > usuario2) {
                            ++itera_Cuac;
                            continue;
                        }
                    }
                }
                break;
            } //fin while

            list<Cuac>::iterator dir_cuac = par.l.insert(itera_Cuac, c); // dir del cuac
            n++;
            return &(*dir_cuac);
        }
    }

    // En caso de que no exista creamos un nuevo Par
    Par nuevo;
    nuevo.nombre = usuario;
    nuevo.l.push_back(c);
    T[h].push_back(nuevo);
    n++;
    return &(T[h].back().l.back()); //ultima dir
}

list<Cuac> TablaHash::follow(string& usuario) {
    cout << "follow " << usuario << endl;

    int h = funcionHash(usuario);
    for (Par &par : T[h]) {
        if (par.nombre == usuario)
            return par.l;
    }
    // en caso de que no exista devuelvo una lista vacia
    list<Cuac> vacia;
    return vacia;
}



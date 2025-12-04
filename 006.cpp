#include<iostream>
#include<list>
#include "006.h"
using namespace std;

// Implementación del Constructor
DiccionarioCuacs::DiccionarioCuacs(){
    //contadorCuacs = 0;
}

// Función auxiliar para comprobar el orden de  inserción

// 004 -> Como queremos que los mensajes se muestren de más a menos recientes, en primer lugar se compara por fecha.
// Un cuac es "anterior" a otro si su fecha es posterior;
// en caso de empate, se compara el texto del mensaje en orden alfabético de menor a mayor;
// y en caso de empate, se compara el nombre del usuario en orden alfabético de menor a mayor.

bool es_anterior(Cuac &nuevo, Cuac &actual){

    Fecha fCuacNuevo = nuevo.get_fecha();
    Fecha fCuacActual = actual.get_fecha();

    if(fCuacActual.es_menor(fCuacNuevo)){
        return true;
    } else if (fCuacNuevo.es_menor(fCuacActual)){
        return false;
    } else {

        // en caso de empate se desempata por orden alfabético del texto
        string tCuacNuevo = nuevo.get_texto();
        string tCuacActual = actual.get_texto();

        if(tCuacNuevo < tCuacActual){
            return true;
        } else if (tCuacActual < tCuacNuevo){
            return false;
        } else{

            // en caso de nuevo empate, se desempata por orden alfabético del nombre de usuario
            if(nuevo.get_usuario() < actual.get_usuario()){
                return true;
            }

            return false;
        }
    }
}

// Método insertar()
void DiccionarioCuacs::insertar(Cuac &nuevo){
    Cuac* pt = tabla.insertar(nuevo); // inserta en tablaHash y almacena dir de memoria
    if (pt != nullptr) {
        arbol.insertar(pt); // si dir no es null, almacena en el AVL
    }
}



// Método last()
void DiccionarioCuacs::last(int N){
    arbol.last(N);
}


// Método follow()

void DiccionarioCuacs::follow(string nombre){
    list<Cuac> lista = tabla.follow(nombre);
    int i = 1;
    for (list<Cuac>::iterator it = lista.begin(); it != lista.end(); it++) {
            cout << i << ". ";
            it->write_cuac();
            cout << "\n";
            i++;
    }
    cout << "Total: " <<lista.size() << " cuac" << endl;
}

// Método date()

void DiccionarioCuacs::date(Fecha fecha, Fecha fecha2){
    arbol.date(fecha, fecha2);
}


#include <iostream>
#include <string>
#include "Mensajes.h"
#include "Cuac.h"

using namespace std;

// Implementación de métodos

// Leer cuac
bool Cuac::read_cuac (string cuac_type_read){

    cuac_type = cuac_type_read;

    if(!(cin >> usuario)){ // if EOF alcanzado
		return false;
	}

    date.leer_fecha();

    if (cuac_type == "mcuac"){

        cin.ignore();
        getline(cin, mensaje);

    } else if (cuac_type == "pcuac"){

        cin >> numero001;
        cin.ignore();

    }

    return true;

}


void Cuac::write_cuac (void){


    cout << usuario << " ";
    date.escribir_fecha();
    cout << "\n";

    if (cuac_type == "mcuac"){

        cout << "   " << mensaje;

    } else if (cuac_type == "pcuac"){

        cout << "   ";
        imprime_mensaje(numero001);

    }

}

// Getters

Fecha Cuac::get_fecha(){
    return date;
}

string Cuac::get_usuario(){
    return usuario;
}

string Cuac::get_texto(){
    if (cuac_type == "mcuac"){
        return mensaje;
    } else if (cuac_type == "pcuac"){
        return obtiene_mensaje(numero001);
    }

    return ""; // en caso de que fuese otra palabra
}


#include <iostream>
#include <string>

#include "003.h"
#include "004.h"
#include "Interprete.h"

using namespace std;

// Constructor vacío
Interprete::Interprete(){}


void Interprete::ejecutarCuacker(){

    string comando;

    // Bucle de control de entradas:

    while(cin >> comando){

        if (comando == "exit"){  // caso de salida
            break;

        } else  if (comando == "mcuac" || comando == "pcuac"){

            Cuac c;

            if( c.read_cuac(comando) ){  // if true

                diccionario.insertar(c);
                cout << diccionario.numElem() << " cuac" << endl;
            }

            } else if (comando == "last"){

                int n;
                cin >> n;
                cout<< comando << " " << n << endl; // imprime mismo comando de entrada en la salida

                diccionario.last(n);

            } else if (comando == "follow"){

                string usuario;
                cin >> usuario;

                diccionario.follow(usuario);

            /*} else if (comando == "tag"){

            string tag;
            cin >> tag;

            if(!tag.empty() && tag[0] == '#'){
                cout << comando << " " << tag << endl;
            }
            */
            } else if (comando == "date"){

                Fecha fecha_inicio, fecha_fin;

                fecha_inicio.leer_fecha();
                fecha_fin.leer_fecha();

                cout << comando << " ";
                fecha_inicio.escribir_fecha();
                cout << " ";
                fecha_fin.escribir_fecha();

                cout << endl;

                diccionario.date(fecha_inicio, fecha_fin);

        }
    }
}




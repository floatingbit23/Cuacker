#include <iostream>
#include <string>
#include "Fecha.h"
#include "Cuac.h"
#include "Interprete.h"

/**
 * @brief Constructor por defecto de nuestro Intérprete.
 */
Interprete::Interprete(){}

/**
 * @brief Ejecutamos el motor de nuestra aplicación.
 * Implementamos un bucle que lee comandos de la entrada estándar y los distribuye
 * a las funciones de nuestro Diccionario.
 */
void Interprete::ejecutarCuacker(){

    std::string comando;

    // Entramos en nuestro bucle de control infinito hasta que se solicite el cierre
    while(std::cin >> comando){

        // Gestionamos el cierre de nuestra sesión
        if (comando == "exit"){

            break; // Salimos del bucle y finaliza el programa

        } else if (comando == "mcuac" || comando == "pcuac"){ // ej. "mcuac john hello world" o "pcuac john 123"

            // Gestionamos la creación de nuevas publicaciones
            Cuac c;

            // Intentamos leer el cuac; si la entrada es correcta, lo incorporamos a nuestro sistema
            if( c.read_cuac(comando) ){

                _servicio_datos.insertar(c); // Insertamos el cuac en nuestro diccionario

                // Informamos al usuario de cuántas publicaciones totales gestionamos ahora
                std::cout << _servicio_datos.numElem() << " cuac" << std::endl;
            }

        } else if (comando == "last"){ // ej. "last 5"

            // Procesamos la solicitud de ver los mensajes más recientes

            int n; // Número de mensajes a mostrar
            std::cin >> n;

            // Confirmamos por pantalla el comando que estamos procesando
            std::cout << comando << " " << n << std::endl;

            _servicio_datos.last(n);

        } else if (comando == "follow"){ // ej. "follow john"

            // Ejecutamos la búsqueda de mensajes de un usuario específico
            std::string usuario;
            std::cin >> usuario;

            _servicio_datos.follow(usuario);

        } else if (comando == "date"){ // ej. "date 01/01/2022 31/12/2022"

            // Filtramos publicaciones en un periodo de tiempo determinado
            Fecha fecha_inicio, fecha_fin;

            // Capturamos los límites temporales
            fecha_inicio.leer_fecha();
            fecha_fin.leer_fecha();

            // Mostramos un eco de la consulta realizada usando nuestro operator<< de Fecha
            std::cout << comando << " " << fecha_inicio << " " << fecha_fin << std::endl;

            _servicio_datos.date(fecha_inicio, fecha_fin);

        } else {
            // Informamos al usuario de que no reconocemos el comando introducido
            std::cerr << "Error: comando '" << comando << "' no reconocido." << std::endl;
        }
    }
}

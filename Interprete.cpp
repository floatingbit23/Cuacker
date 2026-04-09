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

                diccionario.insertar(c); // Insertamos el cuac en nuestro diccionario

                // Informamos al usuario de cuántas publicaciones totales gestionamos ahora
                std::cout << diccionario.numElem() << " cuac" << std::endl;
            }

        } else if (comando == "last"){ // ej. "last 5"

            // Procesamos la solicitud de ver los mensajes más recientes

            int n; // Número de mensajes a mostrar
            std::cin >> n;

            // Confirmamos por pantalla el comando que estamos procesando
            std::cout << comando << " " << n << std::endl;

            diccionario.last(n);

        } else if (comando == "follow"){ // ej. "follow john"

            // Ejecutamos la búsqueda de mensajes de un usuario específico
            std::string usuario;
            std::cin >> usuario;

            diccionario.follow(usuario);

        } else if (comando == "date"){ // ej. "date 01/01/2022 31/12/2022"

            // Filtramos publicaciones en un periodo de tiempo determinado
            Fecha fecha_inicio, fecha_fin;

            // Capturamos los límites temporales
            fecha_inicio.leer_fecha();
            fecha_fin.leer_fecha();

            // Mostramos un eco de la consulta realizada
            std::cout << comando << " ";
            fecha_inicio.escribir_fecha();

            std::cout << " ";
            fecha_fin.escribir_fecha();
            
            std::cout << std::endl;

            diccionario.date(fecha_inicio, fecha_fin);

        }
    }
}




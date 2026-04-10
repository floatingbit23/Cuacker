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

            _servicio_datos.date(fecha_inicio, fecha_fin);

        } else if (comando == "tag"){ // ej. "tag #programacion"

            // Buscamos publicaciones que contengan un hashtag específico
            std::string hashtag;
            std::cin >> hashtag;

            _servicio_datos.tag(hashtag);

        } else if (comando == "search"){ // ej. "search hola"

            // Buscamos publicaciones que contengan una subcadena en su texto
            std::string texto;

            // Leemos toda la línea con getline() para permitir búsquedas con espacios
            std::cin.ignore();
            std::getline(std::cin, texto);

            _servicio_datos.search(texto);

        } else if (comando == "stats"){ // ej. "stats"

            // Mostramos las estadísticas generales del sistema
            _servicio_datos.stats();

        } else if (comando == "help"){ // ej. "help"

            // Mostramos la lista de comandos disponibles
            std::cout << "\n=== Comandos disponibles en Cuacker ===" << std::endl;
            std::cout << "  mcuac <usuario> <fecha> <mensaje>  - Publicar un cuac manual" << std::endl;
            std::cout << "  pcuac <usuario> <fecha> <numero>   - Publicar un cuac predefinido" << std::endl;
            std::cout << "  last <N>                           - Ver los ultimos N cuacs" << std::endl;
            std::cout << "  follow <usuario>                   - Ver cuacs de un usuario" << std::endl;
            std::cout << "  date <fecha_ini> <fecha_fin>       - Cuacs en un rango de fechas" << std::endl;
            std::cout << "  tag <#hashtag>                     - Buscar por hashtag" << std::endl;
            std::cout << "  search <texto>                     - Buscar texto en los cuacs" << std::endl;
            std::cout << "  stats                              - Estadisticas del sistema" << std::endl;
            std::cout << "  help                               - Mostrar esta ayuda" << std::endl;
            std::cout << "  exit                               - Salir de Cuacker" << std::endl;
            std::cout << "=============================\n" << std::endl;

        } else {
            // Informamos al usuario de que no reconocemos el comando introducido
            std::cerr << "Error: comando '" << comando << "' no reconocido. Escribe 'help' para ver la lista de comandos disponibles. " << std::endl;
        }
    }
}

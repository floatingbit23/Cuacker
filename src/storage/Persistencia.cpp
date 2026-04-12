#include "Persistencia.h"
#include <fstream>
#include <iostream>

/**
 * @file Persistencia.cpp
 * @brief Implementación de la capa de serialización CSV para Cuacker.
 */

/**
 * @brief Guarda la lista de cuacs recibida en un archivo de texto plano.
 * Utilizamos el delimitador '|' para separar los campos del objeto Cuac.
 * @param fichero Ruta del archivo donde se guardará la información.
 * @param cuacs Lista de objetos Cuac a persistir.
 */
void Persistencia::guardar(const std::string& fichero, const std::list<Cuac>& cuacs) {

    // Abrimos el flujo de salida hacia el archivo (sobrescribe lo anterior)
    std::ofstream out(fichero);
    
    if (!out) {
        std::cerr << "Error crítico! No se ha podido abrir el archivo [" << fichero << "] para escritura.\n";
        return;
    }

    // Iteramos sobre cada cuac del sistema, O(n)
    for (const Cuac& c : cuacs) {
        // Obtenemos la fecha del cuac
        const Fecha& f = c.get_fecha();
        
        // Escribimos los campos serializados separados por pipes '|'
        // Formato: usuario|dia|mes|anio|hora|minuto|segundo|tipo|mensaje_o_numero

        out << c.get_usuario() << "|"
            << f.get_dia() << "|" << f.get_mes() << "|" << f.get_anio() << "|"
            << f.get_hora() << "|" << f.get_minuto() << "|" << f.get_segundo() << "|"
            << c.get_tipo_cuac() << "|";
            
        // Gestionamos el contenido según el tipo (mensaje manual o índice predefinido)
        if (c.get_tipo_cuac() == "mcuac") {
            out << c.get_texto();
        } else {
            out << c.get_numero_predefinido();
        }
        
        // Terminamos la línea para el siguiente registro
        out << "\n";
    }

    // Cerramos el flujo para asegurar que el SO vuelque los datos al disco
    out.close();
}

/**
 * @brief Reconstruye el estado del sistema leyendo desde un archivo de texto plano (txt, dat, csv, log...)
 * El método lee línea a línea el archivo y parsea cada campo buscando el delimitador '|'.
 * @param fichero Ruta del archivo desde donde cargar los datos.
 * @return list<Cuac> -> Lista de objetos Cuac reconstruidos.
 */
std::list<Cuac> Persistencia::cargar(const std::string& fichero) {

    // Creamos la lista donde guardaremos los cuacs
    std::list<Cuac> cuacs;

    // Abrimos el flujo de entrada hacia el archivo
    std::ifstream in(fichero);
    
    // Si no existe el fichero, simplemente devolvemos la lista vacía (comportamiento por defecto en el primer arranque)
    if (!in) return cuacs; 

    std::string linea;

    // Leemos el archivo línea a línea
    while (std::getline(in, linea)) {

        // Saltamos líneas vacías si las hubiera
        if (linea.empty()) continue; 

        size_t pos = 0;
        std::string campos[8]; // Array temporal para los primeros 8 metadatos

        bool formato_valido = true; // Flag para detectar líneas corruptas
        
        // Fase de Parseo: Buscamos los 8 delimitadores '|' de los metadatos
        for (int i = 0; i < 8; ++i) {
            size_t delim = linea.find('|', pos);

            if (delim == std::string::npos) {
                formato_valido = false; // Si no encontramos un delimitador, la línea está corrupta
                break;
            }

            campos[i] = linea.substr(pos, delim - pos); // Extraemos el campo
            pos = delim + 1; // Saltamos al caracter después del pipe
        }

        if (!formato_valido) continue; // Si la línea está corrupta, pasamos a la siguiente

        // El contenido restante tras el octavo '|' es el cuerpo del cuac (mensaje o número)
        std::string resto = linea.substr(pos);

        // Si el tipo es conocido, intentamos reconstruir el objeto
        if (campos[7] == "mcuac" || campos[7] == "pcuac") {

            try {

                // Convertimos los campos de texto a los tipos de datos correspondientes
                std::string usuario = campos[0];

                // Construimos el objeto Fecha a partir de los campos numéricos (con stoi -> String to Integer)
                Fecha f(std::stoi(campos[1]), std::stoi(campos[2]), std::stoi(campos[3]), 
                        std::stoi(campos[4]), std::stoi(campos[5]), std::stoi(campos[6]));
                
                // Obtenemos el tipo de cuac
                std::string tipo = campos[7];

                int numero = 0; 
                std::string mensaje = "";
                
                // Asignamos según el tipo de origen
                if (tipo == "mcuac") {
                    mensaje = resto;
                } else if (tipo == "pcuac") {
                    numero = std::stoi(resto); 
                }

                // Creamos el objeto Cuac y lo añadimos a la lista (usa el constructor de persistencia)
                Cuac c(usuario, f, tipo, mensaje, numero);
                cuacs.push_back(c); 

            } catch (...) {
                // Si falla la conversión (ej. stoi), ignoramos la línea de forma segura
            }
        }
    }
    
    in.close();
    return cuacs;
}

#pragma once
#include <string>
#include <list>
#include "Cuac.h"

/**
 * @file Persistencia.h
 * @brief Manejador de guardado y carga de la BDD de cuacs.
 * Usamos formato CSV con delimitador '|' para eficiencia y compatibilidad.
 */

class Persistencia {
public:

    /**
     * @brief Guarda una lista de cuacs en un archivo de texto.
     */
    static void guardar(const std::string& fichero, const std::list<Cuac>& cuacs);

    /**
     * @brief Carga todos los cuacs almacenados previamente desde el fichero.
     */
    static std::list<Cuac> cargar(const std::string& fichero);

    // const & (paso por referencia constante) para evitar copias innecesarias    
};

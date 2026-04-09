#pragma once

/**
 * @file TablaHash.h
 * @brief Implementamos nuestro sistema de almacenamiento rápido mediante una Tabla Hash.
 * En esta estructura nos encargamos de organizar los cuacs por usuario para permitir búsquedas y seguimientos ultra eficientes.
 * Tiempo algorítmico O(1)
 */

#include "Cuac.h"
#include <string>
#include <list>

class TablaHash
{
private:
    /**
     * @brief Definimos nuestra estructura Par.
     * En nuestro diseño, asociamos cada nombre de usuario con una lista de sus cuacs.
     */
    class Par {
        std::string nombre;     // Nombre del usuario (nuestra clave)
        std::list<Cuac> l;      // Lista de cuacs realizados por este usuario
        friend class TablaHash;
    };

    std::list<Par>* T; // Array de listas (nuestro "cubo" para manejar colisiones)
    int B;             // Número total de entradas en nuestra tabla (tamaño del array)
    int n;             // Contador total de elementos que hemos insertado

public:
    /**
     * @brief Constructor y destructor para gestionar nuestra memoria dinámica.
     */
    TablaHash();
    ~TablaHash();

    /**
     * @brief Nuestra función de dispersión para calcular la posición en la tabla.
     */
    int funcionHash(std::string& clave);

    /**
     * @brief Insertamos un cuac (copia temporal) y devolvemos su dirección de memoria.
     * Modificamos este método para que nos devuelva un puntero, facilitando la creación de índices.
     */
    Cuac* insertar(Cuac c);

    /**
     * @brief Recuperamos la lista completa de cuacs de un usuario específico (referencia).
     */
    std::list<Cuac> follow(std::string& usuario);

    /**
     * @brief Nos devuelve el número total de elementos insertados.
     */ 
    int nElem(void) { 
        return n; 
    }
};

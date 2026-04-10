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
        std::string _nombre_usuario; // Nombre del usuario (nuestra clave)
        std::list<Cuac> _lista_cuacs; // Lista de cuacs realizados por este usuario
        friend class TablaHash;
    };

    std::list<Par>* _tabla_buckets; // Array de listas (nuestro "cubo/bucket" para manejar colisiones)
    int _num_buckets;               // Número total de entradas en nuestra tabla (tamaño del array)
    int _num_elementos;            // Contador total de elementos que hemos insertado

public:
    /**
     * @brief Constructor y destructor para gestionar nuestra memoria dinámica.
     */
    TablaHash();
    ~TablaHash();

    /**
     * @brief Nuestra función de dispersión para calcular la posición en la tabla.
     */
    int funcionHash(const std::string& clave);

    /**
     * @brief Insertamos un cuac (copia temporal) y devolvemos su dirección de memoria.
     * @param c El objeto Cuac que queremos almacenar.
     * @param esNuevo (Opcional) Se pondrá a true si el usuario no existía previa mente.
     * @return La dirección de memoria donde ha quedado guardado el cuac.
     */
    Cuac* insertar(Cuac c, bool* esNuevo = nullptr);

    /**
     * @brief Recuperamos la lista completa de cuacs de un usuario específico (referencia).
     */
    std::list<Cuac> follow(std::string& usuario);

    /**
     * @brief Comprobamos si un usuario ya existe en nuestra tabla.
     */
    bool existeUsuario(const std::string& usuario);

    /**
     * @brief Nos devuelve el número total de elementos insertados.
     */ 
    int nElem() const { 
        return _num_elementos; 
    }
};

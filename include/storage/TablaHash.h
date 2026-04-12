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
        friend class TablaHash;         // Permitimos acceso a TablaHash para operaciones internas
    };

    std::list<Par>* _tabla_buckets; // Array de listas (nuestro bucket para manejar colisiones)
    int _num_buckets;               // Número total de entradas en nuestra tabla (tamaño del array)
    int _num_elementos;            // Contador total de elementos que hemos insertado
    int _num_usuarios;             // Contador de usuarios unicos (Nivel 3)

    /**
     * @brief Redimensiona dinámicamente la tabla hash para mantener el tiempo algorítmico O(1).
     * Se ejecuta cuando el factor de carga supera el umbral (>0.75).
     */
    void rehash();

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
     * @brief Insertamos un cuac y devolvemos su dirección de memoria permanente.
     * @param c El objeto Cuac que queremos almacenar (pasado por referencia constante para evitar copias).
     * @param esNuevo (Opcional) Se pondrá a true si el usuario no existía previamente.
     * @return La dirección de memoria donde ha quedado guardado el cuac.
     */
    Cuac* insertar(const Cuac& c, bool* esNuevo = nullptr);

    /**
     * @brief Recuperamos la lista completa de cuacs de un usuario específico (referencia).
     * @param usuario Nombre del usuario, sin espacios (const reference).
     * @return Una lista con todos los cuacs del usuario, ordenada cronológicamente.
     */
    std::list<Cuac> follow(const std::string& usuario);

    /**
     * @brief Comprobamos si un usuario ya existe en nuestra tabla.
     * @param usuario Nombre del usuario a comprobar (const reference).
     * @return true si el usuario existe, false en caso contrario.
     */
    bool existeUsuario(const std::string& usuario);

    /**
     * @brief Elimina un Cuac específico conociendo a su autor (O(k_usuario)). 
     * Acceso directo al bucket del usuario.
     * @param usuario Nombre del autor del cuac a borrar.
     * @param id_cuac ID del mensaje a borrar.
     * @return true si se eliminó, false en caso contrario.
     */
    bool eliminar(const std::string& usuario, int id_cuac);


    /**
     * @brief Nos devuelve el número total de elementos insertados.
     */ 
    int nElem() const { 
        return _num_elementos; 
    }
};

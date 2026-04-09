#pragma once

/**
 * @file DiccionarioCuacs.h
 * @brief Definimos nuestra clase controladora principal: el Diccionario de Cuacs.
 * En este módulo orquestamos el uso de nuestras dos estructuras de datos principales
 * (Tabla Hash y Árbol AVL) para gestionar las publicaciones de nuestra red social.
 */

#include "ArbolAVL.h"
#include "Cuac.h"
#include "TablaHash.h"
#include <string>

class DiccionarioCuacs {

private:

  /**
   * @brief Nuestras estructuras de almacenamiento.
   * Usamos la tabla para búsquedas por usuario y el árbol para búsquedas temporales.
   */

  TablaHash tabla;  // Tiempo algorítmico O(1)
  Arbol_AVL arbol;  // Tiempo algorítmico O(log n)

public:
  /**
   * @brief Inicializamos nuestro diccionario.
   */
  DiccionarioCuacs();

  /**
   * @brief Insertamos una nueva publicación en ambas estructuras (TablaHash y ArbolAVL) sincronizadamente.
   */
  void insertar(Cuac &nuevo);

  /**
   * @brief Mostramos los 'N' mensajes más recientes de todo el sistema.
   */
  void last(int N);

  /**
   * @brief Recuperamos y mostramos todos los cuacs de un usuario concreto.
   */
  void follow(std::string nombre);

  /**
   * @brief Filtramos y mostramos mensajes dentro de un rango de fechas.
   */
  void date(Fecha fecha, Fecha fecha2);

  /**
   * @brief Consultamos el número total de publicaciones que gestionamos.
   */
  int numElem() { return tabla.nElem(); }
};

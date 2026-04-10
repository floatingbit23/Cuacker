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
#include <map>
#include <list>

class DiccionarioCuacs {

private:

  /**
   * @brief Nuestras estructuras de almacenamiento.
   * Usamos la tabla para búsquedas por usuario y el árbol para búsquedas temporales.
   */

  TablaHash _tabla_usuarios;  // Tiempo algorítmico O(1)
  Arbol_AVL _arbol_fechas;    // Tiempo algorítmico O(log n)

  /**
   * @brief Índice de hashtags para búsquedas por etiquetas.
   * Mapeamos cada hashtag (sin el '#') a una lista de punteros a los cuacs que lo contienen.
   * Usamos std::map para mantener los hashtags ordenados alfabéticamente.
   */
  std::map<std::string, std::list<Cuac*>> _indice_hashtags; // Tiempo algorítmico O(log n)

  /**
   * @brief Contador de usuarios únicos registrados en el sistema.
   */
  int _num_usuarios_unicos;

  /**
   * @brief Extraemos los hashtags del texto de un cuac.
   * Buscamos tokens que empiezan con '#' y los almacenamos en nuestro índice.
   * @param texto Texto del cuac del que extraer los hashtags.
   * @param cuac_ptr Puntero al cuac que contiene los hashtags.
   */
  void extraer_hashtags(const std::string& texto, Cuac* cuac_ptr);

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
   * @brief Buscamos y mostramos los cuacs que contienen un hashtag específico.
   * @param hashtag El hashtag a buscar (con o sin '#').
   */
  void tag(const std::string& hashtag);

  /**
   * @brief Buscamos y mostramos los cuacs cuyo texto contenga una subcadena.
   * Delegamos la búsqueda en nuestro Árbol AVL.
   * @param texto Subcadena a buscar.
   */
  void search(const std::string& texto);

  /**
   * @brief Mostramos estadísticas generales de nuestra red social.
   * Incluye: total de cuacs, usuarios únicos y hashtags registrados.
   */
  void stats();

  /**
   * @brief Consultamos el número total de publicaciones que gestionamos.
   */
  int numElem() const { return _tabla_usuarios.nElem(); }
};

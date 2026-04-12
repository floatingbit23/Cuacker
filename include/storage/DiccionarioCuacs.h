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
#include <unordered_map>

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
   * KEY: String (Hashtag) -> VALUE: Lista de punteros a Cuacs
   * Mapeamos cada hashtag (sin el '#') a una lista de punteros a los cuacs que lo contienen.
   * Usamos std::map para mantener los hashtags ordenados alfabéticamente.
   */
  std::map<std::string, std::list<Cuac*>> _indice_hashtags; // Tiempo algorítmico O(log n)

  /**
   * @brief Índice de IDs para búsquedas y borrados instantáneos (O(1)).
   * KEY: Entero (ID) -> VALUE: Puntero al Cuac
   */
  std::unordered_map<int, Cuac*> _indice_ids;

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
  void insertar(const Cuac &nuevo);

  /**
   * @brief Elimina una publicación de todo el sistema dado su ID (Nivel 3).
   */
  void eliminar(int id_cuac);

  /**
   * @brief Mostramos los 'N' mensajes más recientes de todo el sistema.
   */
  void last(int N);

  /**
   * @brief Recuperamos y mostramos todos los cuacs de un usuario concreto.
   * @param nombre Nombre del usuario seguido (const reference).
   */
  void follow(const std::string& nombre);

  /**
   * @brief Filtramos y mostramos mensajes dentro de un rango de fechas.
   * @param fecha Fecha inicial (const reference).
   * @param fecha2 Fecha final (const reference).
   */
  void date(const Fecha& fecha, const Fecha& fecha2);

  /**
   * @brief Buscamos y mostramos los cuacs que contienen un hashtag específico.
   * @param hashtag El hashtag a buscar (con o sin '#') (const reference).
   */
  void tag(const std::string& hashtag);

  /**
   * @brief Buscamos y mostramos los cuacs cuyo texto contenga una subcadena.
   * Delegamos la búsqueda en nuestro Árbol AVL.
   * @param texto Subcadena a buscar (const reference).
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

  /**
   * @brief Extrae todos los cuacs para exportarlos a la persistencia.
   * @return Lista de cuacs.
   */
  // NOTA: Esta función Inline es llamada por la capa de persistencia.
  std::list<Cuac> exportar() const { return _tabla_usuarios.exportarCuacs(); }

  /**
   * @brief Restaura el sistema insertando una lista de cuacs guardada.
   * @param cuacs Lista de cuacs a insertar (const reference).
   */
  // NOTA: Esta función es llamada por la capa de persistencia.
  void cargarDesde(const std::list<Cuac>& cuacs);
};

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
#include <unordered_set>

/*
 * Forward declaration para evitar dependencia circular,
 * ya que Persistencia necesita saber qué es un Cuac (porque los almacena), y Cuac necesita saber qué es Persistencia.

 * Esto es necesario para que el compilador sepa que Persistencia existe, aunque no sepa su estructura completa.
 * De esta forma, podemos usar punteros a Persistencia en DiccionarioCuacs (reservar 8 bytes) sin necesidad de incluir el archivo completo.
 */
class Persistencia; 

/**
 * @brief Clase controladora principal: el Diccionario de Cuacs.
 * Orquesta el uso de las 2 estructuras de datos principales
 * (Tabla Hash y Árbol AVL) para gestionar las publicaciones de la red social.
 */

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
   * @brief Contador de usuarios únicos registrados en la red social.
   */
  int _num_usuarios_unicos;

  /**
   * @brief Caché en RAM del grafo social (seguidores).
   * KEY: nombre de usuario -> VALUE: set de usuarios que sigue.
   * Se carga desde SQLite al hacer login y se actualiza en tiempo real.
   */
  std::unordered_map<std::string, std::unordered_set<std::string>> _grafo_seguidores;

  /**
   * @brief Puntero a la capa de persistencia SQLite (inyectado externamente).
   * Se mantiene en 'nullptr' durante la carga inicial para evitar INSERTs redundantes.
   */
  Persistencia* _persistencia;

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
   * @brief Comprobamos si un usuario existe en el sistema.
   * @param nombre Nombre del usuario a comprobar.
   * @return true si existe, false en caso contrario.
   */
  bool existeUsuario(const std::string& nombre) { return _tabla_usuarios.existeUsuario(nombre); }

  /**
   * @brief Inyecta la referencia a la capa de Persistencia.
   * Se llama DESPUÉS de cargar los datos iniciales para activar el auto-guardado.
   * @param p Puntero a la instancia de Persistencia (no es dueño de la instancia).
   */
  void setPersistencia(Persistencia* p);

  // === GRAFO SOCIAL (Seguidores) ===

  /**
   * @brief Carga los seguidos de un usuario desde SQLite a la caché en RAM.
   * @param usuario Nombre del usuario cuyo grafo cargar.
   * @param seguidos Lista de seguidos (obtenida previamente de Persistencia).
   */
  void cargarGrafo(const std::string& usuario, const std::list<std::string>& seguidos);

  /**
   * @brief Registra que 'seguidor' sigue a 'seguido' (RAM + SQLite).
   * @param seguidor Nombre del usuario que sigue.
   * @param seguido Nombre del usuario seguido.
   */
  void seguir(const std::string& seguidor, const std::string& seguido);

  /**
   * @brief Elimina la relación de seguimiento (RAM + SQLite).
   * @param seguidor Nombre del usuario que deja de seguir.
   * @param seguido Nombre del usuario al que se deja de seguir.
   * @return true si la relación existía, false en caso contrario.
   */
  bool dejarDeSeguir(const std::string& seguidor, const std::string& seguido);

  /**
   * @brief Devuelve el set de seguidos de un usuario (desde la caché en RAM).
   * @param usuario Nombre del usuario.
   * @return Referencia al set de seguidos (vacío si no tiene login).
   */
  const std::unordered_set<std::string>& getSeguidos(const std::string& usuario);

  /**
   * @brief Muestra los últimos N cuacs filtrados por los seguidos de un usuario.
   * Incluye los cuacs propios del usuario en el timeline.
   * @param N Número de cuacs a mostrar.
   * @param usuario_activo Nombre del usuario logueado.
   */
  void lastPersonalizado(int N, const std::string& usuario_activo);
};

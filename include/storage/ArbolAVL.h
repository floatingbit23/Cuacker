#pragma once

/**
 * @file ArbolAVL.h
 * @brief Definimos nuestra estructura de Árbol AVL para la gestión temporal de cuacs.
 * El árbol nos permite mantener todos los mensajes ordenados por fecha y hora
 * de forma balanceada, garantizando tiempos de acceso logarítmicos.
 */

#include <list>
#include <unordered_set>
#include <memory>
#include "Cuac.h"

using std::list;
using std::unordered_set;
using std::unique_ptr;
using std::make_unique;

/**
 * @brief Clase Nodo
 * Representa cada punto de unión en nuestro árbol. 
 */
class Nodo {
    friend class Arbol_AVL;

private:
    // Punteros inteligentes a nuestras ramas izquierda y derecha (dueños de la memoria) 
    unique_ptr<Nodo> _hijoIzquierdo; // 8 bytes
    unique_ptr<Nodo> _hijoDerecho;   // 8 bytes

    // Lista de punteros a cuacs con la misma fecha 
    list<Cuac*> _listaCuacs; // 8 bytes (punteros internos)

    // La fecha que identifica a este nodo
    Fecha _fecha; // 24 bytes (4-aligned)

    // Almacenamos la altura para balancear el árbol eficientemente
    int _altura; // 4 bytes (4-aligned)

    /* Padding total inevitable: 4 bytes*/

public:
    /**
     * @brief Creamos un nuevo nodo a partir de un cuac inicial.
     * @param nuevo_cuac Puntero al cuac que se va a insertar.
     * @note Se inicializan los punteros a nullptr, la lista con el cuac, la fecha y la altura en 1,
     * usando lista de inicialización de miembros.
     */
    Nodo(Cuac* nuevo_cuac) 
        : _hijoIzquierdo(nullptr), _hijoDerecho(nullptr), _listaCuacs({nuevo_cuac}), 
          _fecha(nuevo_cuac->get_fecha()), _altura(1) {}

    /**
     * @brief El destructor manual ya no es necesario.
     * Al usar unique_ptr, la limpieza es automática y segura.
     */
    ~Nodo() = default;
};

/**
 * @brief Clase Arbol_AVL
 * Nuestra clase principal para gestionar la jerarquía de nodos.
 */
class Arbol_AVL {


private:
    // El punto de inicio de nuestro árbol (dueño de la jerarquía)
    unique_ptr<Nodo> _raiz; 

    // Métodos internos para gestionar el balanceo y la altura
    // Usamos punteros crudos para consulta (ya que no son dueños)
    int obtener_altura(Nodo* nodo_consulta);
    int obtener_balanceo(Nodo* nodo_consulta);

    // Implementamos los giros necesarios para mantener el equilibrio del AVL
    // Estos métodos transfieren la propiedad del nodo raíz local
    unique_ptr<Nodo> giro_derecha(unique_ptr<Nodo> nodo_raiz_local);
    unique_ptr<Nodo> giro_izquierda(unique_ptr<Nodo> nodo_raiz_local);

    // Funciones recursivas de inserción y búsqueda
    // La inserción y eliminación transfieren propiedad (sink/source)
    unique_ptr<Nodo> insertar_recursivo(unique_ptr<Nodo> nodo_actual, Cuac* nuevo_cuac);
    
    // Las búsquedas solo consultan, por lo que usamos punteros crudos
    void buscar_ultimos_recursivo(Nodo* nodo_actual, int& cuacs_restantes, int& contador_posicion);
    void buscar_por_rango_recursivo(Nodo* nodo_actual, const Fecha& fecha_inicio, const Fecha& fecha_fin, int& contador_total);
    void buscar_texto_recursivo(Nodo* nodo_actual, const std::string& texto, int& contador_total);

    /** @brief Traversal filtrado (derecha→raíz→izquierda) para timeline personalizado. */
    void buscar_ultimos_filtrado_recursivo(Nodo* nodo_actual, int& cuacs_restantes,
        int& contador_posicion, const unordered_set<std::string>& usuarios_permitidos);

    // Funciones de eliminación
    unique_ptr<Nodo> eliminar_recursivo(unique_ptr<Nodo> nodo_actual, int id_cuac, const Fecha& fecha_cuac);
    Nodo* obtener_nodo_minimo(Nodo* nodo_actual);

public:
    /**
     * @brief Constructor y destructor de nuestro árbol.
     */
    Arbol_AVL();
    ~Arbol_AVL();

    /**
     * @brief Inserta un nuevo cuac en el árbol.
     */
    void insertar(Cuac* cuac_a_insertar);

    /**
     * @brief Filtra cuacs por rango de fechas.
     */
    void date(const Fecha& fecha_inicio, const Fecha& fecha_fin);

    /**
     * @brief Muestra los últimos N cuacs.
     */
    void last(int cantidad_a_mostrar);

    /**
     * @brief Busca texto en los cuacs.
     */
    void search(const std::string& texto);

    /**
     * @brief Elimina un cuac por ID y fecha.
     */
    void eliminar(int id_cuac, const Fecha& fecha_cuac);

    /**
     * @brief Muestra los últimos N cuacs filtrados por un conjunto de usuarios.
     * Solo muestra cuacs cuyo autor esté en el set de usuarios permitidos.
     * @param cantidad_a_mostrar Número de cuacs a mostrar.
     * @param usuarios_permitidos Set de nombres de usuarios cuyos cuacs se incluyen.
     */
    void lastFiltrado(int cantidad_a_mostrar, const unordered_set<std::string>& usuarios_permitidos);
};

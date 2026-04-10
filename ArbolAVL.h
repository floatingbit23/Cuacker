#pragma once

/**
 * @file ArbolAVL.h
 * @brief Definimos nuestra estructura de Árbol AVL para la gestión temporal de cuacs.
 * El árbol nos permite mantener todos los mensajes ordenados por fecha y hora
 * de forma balanceada, garantizando tiempos de acceso logarítmicos.
 */
#include <list>
#include "Cuac.h"

/**
 * @brief Clase Nodo
 * Representa cada punto de unión en nuestro árbol. 
 * En nuestro diseño, cada nodo agrupa Cuacs que tienen exactamente la misma fecha.
 */
class Nodo {
    friend class Arbol_AVL;

private:
    // Punteros a nuestras ramas izquierda y derecha
    Nodo *_hijoIzquierdo;
    Nodo *_hijoDerecho;

    // Almacenamos la altura para balancear el árbol eficientemente
    int _altura;

    // Lista de punteros a cuacs con la misma fecha
    std::list<Cuac*> _listaCuacs;

    // La fecha que identifica a este nodo
    Fecha _fecha;

public:
    /**
     * @brief Creamos un nuevo nodo a partir de un cuac inicial.
     */
    Nodo(Cuac* nuevo_cuac) {
        _fecha = nuevo_cuac->get_fecha();
        _listaCuacs.push_back(nuevo_cuac);
        _hijoIzquierdo = nullptr;
        _hijoDerecho = nullptr;
        _altura = 1; // Inicializamos la altura como 1 para nuevos nodos
    }

    /**
     * @brief Destructor recursivo para limpiar nuestro árbol de la memoria.
     */
    ~Nodo() {
        delete _hijoIzquierdo;
        delete _hijoDerecho;
    }
};

/**
 * @brief Clase Arbol_AVL
 * Nuestra clase principal para gestionar la jerarquía de nodos.
 */
class Arbol_AVL {


private:
    Nodo* _raiz; // El punto de inicio de nuestro árbol

    // Métodos internos para gestionar el balanceo y la altura
    int obtener_altura(Nodo* nodo_consulta);
    int obtener_balanceo(Nodo* nodo_consulta);

    // Implementamos los giros necesarios para mantener el equilibrio del AVL
    Nodo* giro_derecha(Nodo* nodo_raiz_local);
    Nodo* giro_izquierda(Nodo* nodo_raiz_local);

    // Funciones recursivas de inserción y búsqueda
    Nodo* insertar_recursivo(Nodo* nodo_actual, Cuac* nuevo_cuac);
    void buscar_ultimos_recursivo(Nodo* nodo_actual, int& cuacs_restantes, int& contador_posicion);
    void buscar_por_rango_recursivo(Nodo* nodo_actual, const Fecha& fecha_inicio, const Fecha& fecha_fin, int& contador_total);
    void buscar_texto_recursivo(Nodo* nodo_actual, const std::string& texto, int& contador_total);

public:
    /**
     * @brief Constructor y destructor de nuestro árbol.
     */
    Arbol_AVL();
    ~Arbol_AVL();

    /**
     * @brief Interfaces públicas para las operaciones de nuestra red social.
     */
    void insertar(Cuac* cuac_a_insertar);
    void date(const Fecha& fecha_inicio, const Fecha& fecha_fin);
    void last(int cantidad_a_mostrar);
    void search(const std::string& texto);
};

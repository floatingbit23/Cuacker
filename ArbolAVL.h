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
    Nodo *hijoI, *hijoD;        // Punteros a nuestras ramas izquierda y derecha
    int altura;                 // Almacenamos la altura para balancear el árbol eficientemente
    std::list<Cuac*> lcuac;     // Lista de punteros a cuacs con la misma fecha
    Fecha fecha;                // La fecha que identifica a este nodo
    bool ordenada;

public:
    /**
     * @brief Creamos un nuevo nodo a partir de un cuac inicial.
     */
    Nodo(Cuac* c) {
        ordenada = true;
        fecha = c->get_fecha();
        lcuac.push_back(c);
        hijoI = nullptr;
        hijoD = nullptr;
        altura = 1; // Inicializamos la altura como 1 para nuevos nodos
    }

    /**
     * @brief Destructor recursivo para limpiar nuestro árbol de la memoria.
     */
    ~Nodo() {
        delete hijoI;
        delete hijoD;
    }
};

/**
 * @brief Clase Arbol_AVL
 * Nuestra clase principal para gestionar la jerarquía de nodos.
 */
class Arbol_AVL {
private:
    Nodo* raiz; // El punto de inicio de nuestro árbol

    // Métodos internos para gestionar el balanceo y la altura
    int get_Altura(Nodo* nodo);
    int get_Balanceo(Nodo* nodo);

    // Implementamos los giros necesarios para mantener el equilibrio del AVL
    Nodo* giro_Derecha(Nodo* nodoD);
    Nodo* giro_Izquierda(Nodo* nodoI);

    // Funciones recursivas de inserción y búsqueda
    Nodo* insertar(Nodo* nodo, Cuac* clave);
    void last(Nodo* nodo, int& restantes, int& contador);
    void date(Nodo* nodo, Fecha fecha, Fecha fecha2, int& contador);

public:
    /**
     * @brief Constructor y destructor de nuestro árbol.
     */
    Arbol_AVL();
    ~Arbol_AVL();

    /**
     * @brief Interfaces públicas para las operaciones de nuestra red social.
     */
    void insertar(Cuac* c);
    void date(Fecha fecha, Fecha fecha2);
    void last(int n);
};

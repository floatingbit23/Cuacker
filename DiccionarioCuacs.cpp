#include <iostream>
#include <list>
#include <string>
#include "DiccionarioCuacs.h"

/**
 * @brief Constructor de nuestra clase Diccionario
 * Por ahora no necesitamos inicialización especial más allá de la de sus miembros (TablaHash y ArbolAVL)
 */
DiccionarioCuacs::DiccionarioCuacs(){
}

/**
 * @brief Implementamos el proceso de inserción dual.
 * Primero guardamos el cuac en nuestra Tabla Hash (que es la dueña del objeto)
 * y usamos el puntero devuelto para indexarlo en nuestro Árbol AVL.
 * @param nuevo Referencia al objeto Cuac que queremos insertar.
 */
void DiccionarioCuacs::insertar(Cuac &nuevo){
    // Insertamos en la tabla y obtenemos la dirección de memoria persistente
    Cuac* pt = _tabla_usuarios.insertar(nuevo); 
    
    // Si la inserción fue exitosa, procedemos a indexarlo en nuestro árbol cronológico
    if (pt != nullptr) {
        _arbol_fechas.insertar(pt);
    }
}

/**
 * @brief Ejecutamos la búsqueda de los últimos 'N' mensajes.
 * Delegamos esta tarea a nuestro árbol, que está optimizado para recorridos temporales.
 * @param N Cantidad de mensajes a mostrar.
 */
void DiccionarioCuacs::last(int N){
    _arbol_fechas.last(N);
}

/**
 * @brief Gestionamos la recuperación de publicaciones de un usuario.
 * Consultamos directamente nuestra Tabla Hash para obtener los resultados de forma casi instantánea.
 * @param nombre Nombre del usuario cuyos cuacs queremos recuperar.
 */
void DiccionarioCuacs::follow(std::string nombre){

    // Obtenemos la lista de cuacs del usuario
    std::list<Cuac> lista = _tabla_usuarios.follow(nombre);
    int i = 1;
    
    // Recorremos la lista obtenida para mostrar cada cuac siguiendo nuestro formato
    for (std::list<Cuac>::iterator it = lista.begin(); it != lista.end(); it++) {
            std::cout << i << ". "; 
            it->write_cuac(); 
            std::cout << "\n";
            i++;
    }
    // Ej. 
    // 1. Hola, este es mi primer cuac
    // 2. ¡Qué día tan maravilloso para programar!
    // 3. Me encanta aprender cosas nuevas
    // Total: 3 cuac

    std::cout << "Total: " << lista.size() << " cuac" << std::endl; // Ejemplo: "Total: 3 cuac"
}

/**
 * @brief Filtramos las publicaciones por rango de fechas.
 * Aprovechamos la estructura de nuestro árbol para encontrar eficientemente los nodos relevantes.
 * @param fecha Límite inferior del rango de búsqueda.
 * @param fecha2 Límite superior del rango de búsqueda.
 */
void DiccionarioCuacs::date(Fecha fecha, Fecha fecha2){ 
    // Ej. date 01/01/2022 31/12/2022
    _arbol_fechas.date(fecha, fecha2); 
}

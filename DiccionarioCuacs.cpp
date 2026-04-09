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
 * @brief Función auxiliar para determinar el orden de prevalencia entre dos Cuacs.
 * Establecemos nuestros criterios de ordenación: primero la fecha (más reciente primero), luego el texto (orden alfabético) y finalmente el usuario (orden alfabético).
 * @return true si 'nuevo' debe ir antes que 'actual' según nuestra jerarquía de ordenación.
 */
bool es_anterior(Cuac &nuevo, Cuac &actual){

    Fecha fCuacNuevo = nuevo.get_fecha(); // Obtenemos la fecha del cuac nuevo
    Fecha fCuacActual = actual.get_fecha(); // Obtenemos la fecha del cuac actual

    // Si el cuac actual es menor (más antiguo) que el nuevo, el nuevo tiene prioridad
    if(fCuacActual.es_menor(fCuacNuevo)){
        return true;

    } else if (fCuacNuevo.es_menor(fCuacActual)){
        return false;

    } else {

        // Si publicaron al mismo tiempo, desempatamos alfabéticamente por el texto
        std::string tCuacNuevo = nuevo.get_texto();
        std::string tCuacActual = actual.get_texto();

        if(tCuacNuevo < tCuacActual){
            return true;

        } else if (tCuacActual < tCuacNuevo){
            return false;

        } else{

            // Como último recurso, comparamos los nombres de los usuarios para desempatar
            if(nuevo.get_usuario() < actual.get_usuario()){
                return true;
            }
            return false;
        }
    }
}

/**
 * @brief Implementamos el proceso de inserción dual.
 * Primero guardamos el cuac en nuestra Tabla Hash (que es la dueña del objeto)
 * y usamos el puntero devuelto para indexarlo en nuestro Árbol AVL.
 */
void DiccionarioCuacs::insertar(Cuac &nuevo){
    // Insertamos en la tabla y obtenemos la dirección de memoria persistente
    Cuac* pt = tabla.insertar(nuevo); 
    
    // Si la inserción fue exitosa, procedemos a indexarlo en nuestro árbol cronológico
    if (pt != nullptr) {
        arbol.insertar(pt);
    }
}

/**
 * @brief Ejecutamos la búsqueda de los últimos 'N' mensajes.
 * Delegamos esta tarea a nuestro árbol, que está optimizado para recorridos temporales.
 */
void DiccionarioCuacs::last(int N){
    arbol.last(N);
}

/**
 * @brief Gestionamos la recuperación de publicaciones de un usuario.
 * Consultamos directamente nuestra Tabla Hash para obtener los resultados de forma casi instantánea.
 */
void DiccionarioCuacs::follow(std::string nombre){

    // Obtenemos la lista de cuacs del usuario
    std::list<Cuac> lista = tabla.follow(nombre);
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
 */
void DiccionarioCuacs::date(Fecha fecha, Fecha fecha2){ 
    // Ej. date 01/01/2022 31/12/2022
    arbol.date(fecha, fecha2); 
}


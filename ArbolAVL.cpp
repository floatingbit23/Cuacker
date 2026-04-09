#include "ArbolAVL.h"
#include <list>
#include <iostream>
#include <algorithm> // Usamos std::max

/**
 * @brief Inicializamos nuestro Árbol AVL.
 * Comenzamos con nuestra raíz apuntando a la nada hasta que insertemos el primer elemento.
 */
Arbol_AVL::Arbol_AVL() {
    raiz = nullptr;
}

/**
 * @brief Liberamos toda la estructura de memoria.
 * Al borrar nuestra raíz, se desencadena la destrucción recursiva de todos nuestros nodos.
 */
Arbol_AVL::~Arbol_AVL() {
    delete raiz;
}

/**
 * @brief Consultamos la altura de un nodo de forma segura.
 * Nos encargamos de devolver 0 si el nodo no existe, evitando accesos ilegales.
 * @return La altura del nodo (cantidad de niveles hacia abajo).
 */
int Arbol_AVL::get_Altura(Nodo* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    return nodo->altura;
}

/**
 * @brief Calculamos el factor de equilibrio de un nodo.
 * Obtenemos la diferencia entre nuestras ramas izquierda y derecha para saber hacia dónde inclinar el balanceo.
 * @return Un entero que indica el grado de desequilibrio (positivo: pesado a la izquierda, negativo: pesado a la derecha).
 */
int Arbol_AVL::get_Balanceo(Nodo* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    return (get_Altura(nodo->hijoI) - get_Altura(nodo->hijoD));
}

/**
 * @brief Realizamos un giro a la derecha en nuestro árbol.
 * Este movimiento nos permite rebalancear la estructura cuando nuestra rama izquierda pesa demasiado.
 * @return Un puntero al nuevo nodo raíz de esta rama tras la rotación.
 */
Nodo* Arbol_AVL::giro_Derecha(Nodo* nodoD) {
    Nodo* nodoHI = nodoD->hijoI; 
    Nodo* nodoHD = nodoHI->hijoD; 

    // Reorganizamos nuestros punteros
    nodoHI->hijoD = nodoD;
    nodoD->hijoI = nodoHD;

    // Actualizamos las alturas de los nodos que hemos movido
    nodoD->altura = std::max(get_Altura(nodoD->hijoI), get_Altura(nodoD->hijoD)) + 1;
    nodoHI->altura = std::max(get_Altura(nodoHI->hijoI), get_Altura(nodoHI->hijoD)) + 1;

    return nodoHI; // Nuestro antiguo hijo izquierdo se convierte en la nueva raíz local
}

/**
 * @brief Ejecutamos un giro a la izquierda.
 * Aplicamos este giro cuando es nuestra rama derecha la que rompe el equilibrio del árbol.
 * @return Un puntero a la nueva raíz local de la rama después del giro.
 */
Nodo* Arbol_AVL::giro_Izquierda(Nodo* nodoI) {
    Nodo* nodoHD = nodoI->hijoD; 
    Nodo* nodoHI = nodoHD->hijoI; 

    nodoHD->hijoI = nodoI;
    nodoI->hijoD = nodoHI;

    nodoI->altura = std::max(get_Altura(nodoI->hijoI), get_Altura(nodoI->hijoD)) + 1;
    nodoHD->altura = std::max(get_Altura(nodoHD->hijoI), get_Altura(nodoHD->hijoD)) + 1;

    return nodoHD; // Elevamos el hijo derecho a la posición superior
}

/**
 * @brief Implementamos nuestra lógica de inserción balanceada.
 * Decidimos usar listas de punteros dentro de cada nodo para optimizar el tiempo
 * cuando varios cuacs coinciden exactamente en la misma fecha.
 * @return El puntero al nodo (posiblemente cambiado por rotaciones) donde se ha realizado la inserción.
 */
Nodo* Arbol_AVL::insertar(Nodo* nodo, Cuac* clave) {
    // Caso base: si llegamos a un hueco vacío, creamos nuestro nuevo nodo
    if (nodo == nullptr) {
        return new Nodo(clave);
    }

    Fecha fechaC = clave->get_fecha();

    // Navegamos por el árbol comparando fechas
    if (fechaC.es_menor(nodo->fecha)) {
        nodo->hijoI = insertar(nodo->hijoI, clave); 
    } else if (nodo->fecha.es_menor(fechaC)) {
        nodo->hijoD = insertar(nodo->hijoD, clave); 
    } else {
        // Si las fechas son idénticas, insertamos el cuac ordenadamente dentro de la lista del nodo
        bool insertado = false;
        for (std::list<Cuac*>::iterator itera_cuac = nodo->lcuac.begin(); itera_cuac != nodo->lcuac.end(); ++itera_cuac) {
            Cuac* EA = *itera_cuac;
            // Desempatamos por texto y luego por usuario para mantener un orden total
            if (clave->get_texto() < EA->get_texto()) {
                nodo->lcuac.insert(itera_cuac, clave);
                insertado = true;
                break;
            } else if (clave->get_texto() == EA->get_texto()) {
                if (clave->get_usuario() < EA->get_usuario()) {
                    nodo->lcuac.insert(itera_cuac,clave);
                    insertado = true;
                    break;
                }
            }
        }
        if (!insertado) {
            nodo->lcuac.push_back(clave);
        }
        return nodo; 
    }

    // Tras insertar, actualizamos la altura de nuestro nodo actual
    nodo->altura = std::max(get_Altura(nodo->hijoI), get_Altura(nodo->hijoD)) + 1;

    // Comprobamos si nos hemos desbalanceado y aplicamos las rotaciones necesarias
    int balanceo = get_Balanceo(nodo);

    // Caso Izquierda-Izquierda
    if (balanceo > 1 && fechaC.es_menor(nodo->hijoI->fecha)) {
        return giro_Derecha(nodo);
    }
    // Caso Derecha-Derecha
    if (balanceo < -1 && nodo->hijoD->fecha.es_menor(fechaC)) {
        return giro_Izquierda(nodo);
    }
    // Caso Izquierda-Derecha
    if (balanceo > 1 && nodo->hijoI->fecha.es_menor(fechaC)) {
        nodo->hijoI = giro_Izquierda(nodo->hijoI);
        return giro_Derecha(nodo);
    }
    // Caso Derecha-Izquierda
    if (balanceo < -1 && fechaC.es_menor(nodo->hijoD->fecha)) {
        nodo->hijoD = giro_Derecha(nodo->hijoD);
        return giro_Izquierda(nodo);
    }

    return nodo;
}

/**
 * @brief Mostramos los 'N' cuacs más recientes de nuestro sistema.
 * Recorremos el árbol de derecha a izquierda (en orden inverso) para 
 * priorizar las fechas más tardías.
 */
void Arbol_AVL::last(Nodo* nodo, int& restantes, int& contador) {
    if (nodo == nullptr || restantes <= 0) {
        return;
    }

    // Primero visitamos la rama derecha (mensajes más futuros)
    last(nodo->hijoD, restantes, contador);

    // Procesamos la lista de mensajes de este nodo
    if (restantes > 0) {
        for (Cuac* c : nodo->lcuac) {
            if (restantes <= 0) break;
            std::cout << contador << ". ";
            c->write_cuac();
            std::cout << "\n";
            contador++;
            restantes--;
        }
    }

    // Finalmente, si aún nos quedan mensajes por mostrar, exploramos la rama izquierda
    if (restantes > 0) {
        last(nodo->hijoI, restantes, contador);
    }
}

/**
 * @brief Buscamos y mostramos mensajes dentro de un rango de fechas.
 */
void Arbol_AVL::date(Nodo* nodo, Fecha fecha, Fecha fecha2, int& contador) {
    if (nodo == nullptr) return;

    // Si el nodo puede tener fechas relevantes a la derecha, exploramos
    if (nodo->fecha.es_menor(fecha2) || nodo->fecha.es_igual(fecha2)) {
        date(nodo->hijoD, fecha, fecha2, contador);
    }

    // Comprobamos si la fecha de nuestro nodo está dentro del intervalo solicitado
    if (!(nodo->fecha.es_menor(fecha)) && !(fecha2.es_menor(nodo->fecha))) {
        for (Cuac* c : nodo->lcuac) {
            contador++;
            std::cout << contador << ". ";
            c->write_cuac();
            std::cout << "\n";
        }
    }

    // Exploramos la rama izquierda si es posible encontrar fechas mayores que el límite inferior
    if (fecha.es_menor(nodo->fecha) || fecha.es_igual(nodo->fecha)) {
        date(nodo->hijoI, fecha, fecha2, contador);
    }
}

/**
 * @brief Métodos envoltorio (wrappers) para facilitar el uso desde nuestra aplicación principal.
 */
void Arbol_AVL::insertar(Cuac* c) {
    raiz = insertar(raiz, c);
}

void Arbol_AVL::date(Fecha fecha, Fecha fecha2) {
    int contador = 0;
    date(raiz, fecha, fecha2, contador);
    std::cout << "Total: " << contador << " cuac" << "\n";
}

void Arbol_AVL::last(int n) {
    int restantes = n;
    int contador = 1;
    last(raiz, restantes, contador);
    int totalMostrados = n - std::max(0, restantes);
    std::cout << "Total: " << totalMostrados << " cuac" << std::endl;
}

#include "ArbolAVL.h"
#include <list>
#include <iostream>
#include <algorithm> // Usamos std::max

/**
 * @brief Inicializamos nuestro Árbol AVL.
 * Comenzamos con nuestra raíz apuntando a la nada (nullptr) hasta que insertemos el primer elemento.
 */
Arbol_AVL::Arbol_AVL() { // Constructor
    _raiz = nullptr; // Inicializamos la raíz a nullptr
}

/**
 * @brief Liberamos toda la estructura de memoria.
 * Al borrar nuestra raíz, se desencadena la destrucción recursiva de todos nuestros nodos.
 */
Arbol_AVL::~Arbol_AVL() {
    delete _raiz;
}

/**
 * @brief Consultamos la altura de un nodo de forma segura.
 * Nos encargamos de devolver 0 si el nodo no existe, evitando accesos ilegales.
 * @param nodo_consulta Puntero al nodo cuya altura queremos conocer.
 * @return La altura del nodo (cantidad de niveles hacia abajo).
 */
int Arbol_AVL::obtener_altura(Nodo* nodo_consulta) {

    // Tratamos el caso base de un nodo nulo
    if (nodo_consulta == nullptr) {
        return 0; 
    }

    // Devolvemos la altura del nodo
    return nodo_consulta->_altura;
}

/**
 * @brief Calculamos el factor de equilibrio de un nodo.
 * Obtenemos la diferencia entre nuestras ramas izquierda y derecha (lo que llamamos altura) para saber hacia dónde inclinar el balanceo.
 * @param nodo_consulta Puntero al nodo cuyo balanceo queremos calcular.
 * @return Un entero que indica el grado de desequilibrio (positivo: pesado a la izquierda, negativo: pesado a la derecha).
 */
int Arbol_AVL::obtener_balanceo(Nodo* nodo_consulta) {

    // Tratamos el caso base de un nodo nulo
    if (nodo_consulta == nullptr) {
        return 0;
    }

    // Calculamos el factor de equilibrio
    return (obtener_altura(nodo_consulta->_hijoIzquierdo) - obtener_altura(nodo_consulta->_hijoDerecho));
}

// === GIROS DEL ÁRBOL ===

/**
 * @brief Realizamos un giro a la derecha en nuestro árbol.
 * Este movimiento nos permite rebalancear la estructura cuando nuestra rama izquierda pesa demasiado.
 * @param nodo_raiz_local Raíz del subárbol que ha perdido el equilibrio.
 * @return Un puntero al nuevo nodo raíz de esta rama tras la rotación.
 */
Nodo* Arbol_AVL::giro_derecha(Nodo* nodo_raiz_local) {
    Nodo* nuevo_hijo_izquierdo = nodo_raiz_local->_hijoIzquierdo; 
    Nodo* sub_arbol_derecho = nuevo_hijo_izquierdo->_hijoDerecho; 

    // Reorganizamos nuestros punteros
    nuevo_hijo_izquierdo->_hijoDerecho = nodo_raiz_local;
    nodo_raiz_local->_hijoIzquierdo = sub_arbol_derecho;

    // Actualizamos las alturas de los nodos que hemos movido
    nodo_raiz_local->_altura = std::max(obtener_altura(nodo_raiz_local->_hijoIzquierdo), obtener_altura(nodo_raiz_local->_hijoDerecho)) + 1;
    nuevo_hijo_izquierdo->_altura = std::max(obtener_altura(nuevo_hijo_izquierdo->_hijoIzquierdo), obtener_altura(nuevo_hijo_izquierdo->_hijoDerecho)) + 1;

    return nuevo_hijo_izquierdo; // El antiguo hijo izquierdo se convierte en la nueva raíz local
}

/**
 * @brief Ejecutamos un giro a la izquierda.
 * Aplicamos este giro cuando es nuestra rama derecha la que rompe el equilibrio del árbol.
 * @param nodo_raiz_local Raíz del subárbol que ha perdido el equilibrio.
 * @return Un puntero a la nueva raíz local de la rama después del giro.
 */
Nodo* Arbol_AVL::giro_izquierda(Nodo* nodo_raiz_local) {
    Nodo* nuevo_hijo_derecho = nodo_raiz_local->_hijoDerecho; 
    Nodo* sub_arbol_izquierdo = nuevo_hijo_derecho->_hijoIzquierdo; 

    nuevo_hijo_derecho->_hijoIzquierdo = nodo_raiz_local;
    nodo_raiz_local->_hijoDerecho = sub_arbol_izquierdo;

    nodo_raiz_local->_altura = std::max(obtener_altura(nodo_raiz_local->_hijoIzquierdo), obtener_altura(nodo_raiz_local->_hijoDerecho)) + 1;
    nuevo_hijo_derecho->_altura = std::max(obtener_altura(nuevo_hijo_derecho->_hijoIzquierdo), obtener_altura(nuevo_hijo_derecho->_hijoDerecho)) + 1;

    return nuevo_hijo_derecho; // Elevamos el hijo derecho a la posición superior
}

// === INSERCIÓN RECURSIVA ===

/**
 * @brief Implementamos nuestra lógica de inserción balanceada.
 * Decidimos usar listas de punteros dentro de cada nodo para optimizar el tiempo cuando varios cuacs coinciden exactamente en la misma fecha.
 * Los Cuacs reales ya están guardados en la TablaHash (_tabla_buckets_), por lo que aquí solo guardamos punteros a ellos.
 * @param nodo_actual Puntero al nodo actual del árbol.
 * @param nuevo_cuac Puntero al cuac que se desea insertar.
 * @return El puntero al nodo (posiblemente cambiado por rotaciones) donde se ha realizado la inserción.
 */
Nodo* Arbol_AVL::insertar_recursivo(Nodo* nodo_actual, Cuac* nuevo_cuac) {

    // CASO BASE: si llegamos a un hueco vacío, creamos nuestro nuevo nodo ahí
    if (nodo_actual == nullptr) {
        return new Nodo(nuevo_cuac);
    }

    // Obtenemos la fecha del cuac que queremos insertar
    const Fecha& fecha_nuevo = nuevo_cuac->get_fecha();

    // Navegamos por el árbol comparando fechas

    // Si la fecha del nuevo cuac es menor que la del nodo actual, nos movemos a la izquierda
    if (fecha_nuevo < nodo_actual->_fecha) {
        nodo_actual->_hijoIzquierdo = insertar_recursivo(nodo_actual->_hijoIzquierdo, nuevo_cuac); 

    // Si la fecha del nuevo cuac es mayor que la del nodo actual, nos movemos a la derecha
    } else if (nodo_actual->_fecha < fecha_nuevo) {
        nodo_actual->_hijoDerecho = insertar_recursivo(nodo_actual->_hijoDerecho, nuevo_cuac); 

    // Si las fechas son idénticas, insertamos el cuac ordenadamente dentro de la lista del nodo
    } else {
        // Inicializamos un booleano para saber si hemos insertado el cuac
        bool insertado = false; 

        // Recorremos la lista de cuacs del nodo actual
        for (std::list<Cuac*>::iterator it_lista = nodo_actual->_listaCuacs.begin(); it_lista != nodo_actual->_listaCuacs.end(); ++it_lista) {
            
            // Obtenemos el cuac existente
            Cuac* cuac_existente = *it_lista;
            
            // Desempatamos por texto para mantener el orden
            // Si el texto del nuevo cuac es menor que el del cuac existente, lo insertamos antes
            if (nuevo_cuac->get_texto() < cuac_existente->get_texto()) {
                nodo_actual->_listaCuacs.insert(it_lista, nuevo_cuac);
                insertado = true; // Marcamos como insertado
                break;

            // Si el texto es igual, desempatamos por usuario
            } else if (nuevo_cuac->get_texto() == cuac_existente->get_texto()) {

                // Si el usuario es menor, lo insertamos antes
                if (nuevo_cuac->get_usuario() < cuac_existente->get_usuario()) {
                    nodo_actual->_listaCuacs.insert(it_lista, nuevo_cuac);
                    insertado = true; // Marcamos como insertado
                    break;
                }
            }
        }

        // Si no hemos insertado el cuac, lo insertamos al final
        if (!insertado) {
            nodo_actual->_listaCuacs.push_back(nuevo_cuac); 
        }

        // Devolvemos el nodo actual
        return nodo_actual; 
    }

    // Tras insertar, actualizamos la altura de nuestro nodo actual
    // (la altura de un nodo es 1 + la altura del mayor de sus hijos)
    nodo_actual->_altura = std::max(obtener_altura(nodo_actual->_hijoIzquierdo), obtener_altura(nodo_actual->_hijoDerecho)) + 1;

    // Comprobamos si nos hemos desbalanceado y aplicamos las rotaciones necesarias (si fuera necesario)
    int factor_balanceo = obtener_balanceo(nodo_actual);

    // === CASOS DE DESBALANCEO ===

    // == CASO IZQUIERDA-IZQUIERDA == 
    
    if (factor_balanceo > 1  // Si el factor de balanceo es mayor que 1, significa que la rama izquierda es más alta
        &&
        fecha_nuevo < nodo_actual->_hijoIzquierdo->_fecha) { // Si la fecha del nuevo cuac es menor que la del hijo izquierdo, significa que el desbalanceo está en la rama izquierda

        return giro_derecha(nodo_actual); // Aplicamos un giro a la derecha
    }

    // == CASO DERECHA-DERECHA ==

    
    if (factor_balanceo < -1  // Si el factor de balanceo es menor que -1, significa que la rama derecha es más alta
        &&
        nodo_actual->_hijoDerecho->_fecha < fecha_nuevo) { // Si la fecha del nuevo cuac es mayor que la del hijo derecho, significa que el desbalanceo está en la rama derecha

        return giro_izquierda(nodo_actual); // Aplicamos un giro a la izquierda
    }

    // == CASO IZQUIERDA-DERECHA ==

    if (factor_balanceo > 1  // Si el factor de balanceo es mayor que 1, significa que la rama izquierda es más alta
        &&
         nodo_actual->_hijoIzquierdo->_fecha < fecha_nuevo) { // Si la fecha del nuevo cuac es mayor que la del hijo izquierdo, significa que el desbalanceo está en la rama izquierda

        nodo_actual->_hijoIzquierdo = giro_izquierda(nodo_actual->_hijoIzquierdo); // Aplicamos, en primer lugar, un giro a la izquierda
        return giro_derecha(nodo_actual); // Aplicamos, en segundo lugar, un giro a la derecha
    }

    // == CASO DERECHA-IZQUIERDA ==
    
    if (factor_balanceo < -1  // Si el factor de balanceo es menor que -1, significa que la rama derecha es más alta
        &&
        fecha_nuevo < nodo_actual->_hijoDerecho->_fecha) { // Si la fecha del nuevo cuac es menor que la del hijo derecho, significa que el desbalanceo está en la rama derecha
        
        nodo_actual->_hijoDerecho = giro_derecha(nodo_actual->_hijoDerecho); // Aplicamos, en primer lugar, un giro a la derecha
        return giro_izquierda(nodo_actual); // Aplicamos, en segundo lugar, un giro a la izquierda
    }

    return nodo_actual; // Devolvemos el nodo actual
}

/**
 * @brief Mostramos los 'N' cuacs más recientes de nuestro sistema.
 * Recorremos el árbol de derecha a izquierda (en orden inverso) para 
 * priorizar las fechas más tardías.
 * @param nodo_actual Nodo desde el que empezamos a buscar (normalmente la raíz).
 * @param cuacs_restantes Referencia al contador de mensajes que nos quedan por mostrar.
 * @param contador_posicion Referencia al número de orden en la lista de visualización.
 */
void Arbol_AVL::buscar_ultimos_recursivo(Nodo* nodo_actual, int& cuacs_restantes, int& contador_posicion) {
    if (nodo_actual == nullptr || cuacs_restantes <= 0) {
        return;
    }

    // Primero visitamos la rama derecha (mensajes más futuros)
    buscar_ultimos_recursivo(nodo_actual->_hijoDerecho, cuacs_restantes, contador_posicion);

    // Procesamos la lista de mensajes de este nodo
    if (cuacs_restantes > 0) {
        for (Cuac* cuac : nodo_actual->_listaCuacs) {
            if (cuacs_restantes <= 0) break;
            std::cout << contador_posicion << ". ";
            cuac->write_cuac();
            std::cout << "\n";
            contador_posicion++;
            cuacs_restantes--;
        }
    }

    // Finalmente, si aún nos quedan mensajes por mostrar, exploramos la rama izquierda
    if (cuacs_restantes > 0) {
        buscar_ultimos_recursivo(nodo_actual->_hijoIzquierdo, cuacs_restantes, contador_posicion);
    }
}

/**
 * @brief Buscamos y mostramos mensajes dentro de un rango de fechas.
 * @param nodo_actual Nodo actual de la búsqueda recursiva.
 * @param fecha_inicio Límite inferior del rango temporal.
 * @param fecha_fin Límite superior del rango temporal.
 * @param contador_total Referencia al contador de cuántos cuacs hemos encontrado.
 */
void Arbol_AVL::buscar_por_rango_recursivo(Nodo* nodo_actual, const Fecha& fecha_inicio, const Fecha& fecha_fin, int& contador_total) {
    if (nodo_actual == nullptr) return;

    // Si el nodo puede tener fechas relevantes a la derecha, exploramos
    if (nodo_actual->_fecha <= fecha_fin) {
        buscar_por_rango_recursivo(nodo_actual->_hijoDerecho, fecha_inicio, fecha_fin, contador_total);
    }

    // Comprobamos si la fecha de nuestro nodo está dentro del intervalo solicitado
    if (nodo_actual->_fecha >= fecha_inicio && nodo_actual->_fecha <= fecha_fin) {
        for (Cuac* cuac : nodo_actual->_listaCuacs) {
            contador_total++;
            std::cout << contador_total << ". ";
            cuac->write_cuac();
            std::cout << "\n";
        }
    }

    // Exploramos la rama izquierda si es posible encontrar fechas mayores que el límite inferior
    if (nodo_actual->_fecha >= fecha_inicio) {
        buscar_por_rango_recursivo(nodo_actual->_hijoIzquierdo, fecha_inicio, fecha_fin, contador_total);
    }
}

/**
 * @brief Método inicializador de inserción de un nuevo cuac en el árbol.
 * @param cuac_a_insertar Puntero al cuac que se desea insertar.
 */
void Arbol_AVL::insertar(Cuac* cuac_a_insertar) {
    _raiz = insertar_recursivo(_raiz, cuac_a_insertar); // Llamamos al método recursivo
}

/**
 * @brief Método inicializador de búsqueda de cuacs por rango de fechas.
 * @param fecha_inicio Fecha de inicio del rango.
 * @param fecha_fin Fecha de fin del rango.
 */
void Arbol_AVL::date(const Fecha& fecha_inicio, const Fecha& fecha_fin) {
    int contador_total = 0;
    buscar_por_rango_recursivo(_raiz, fecha_inicio, fecha_fin, contador_total);
    std::cout << "Total: " << contador_total << " cuac" << "\n";
}

/**
 * @brief Método inicializador de búsqueda de los últimos N cuacs.
 * @param cantidad_a_mostrar Número de cuacs a mostrar.
 */
void Arbol_AVL::last(int cantidad_a_mostrar) {
    int restantes = cantidad_a_mostrar;
    int contador_posicion = 1;
    buscar_ultimos_recursivo(_raiz, restantes, contador_posicion);
    int total_mostrados = cantidad_a_mostrar - std::max(0, restantes);
    std::cout << "Total: " << total_mostrados << " cuac" << std::endl;
}

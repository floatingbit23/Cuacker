#include "ArbolAVL.h"
#include <list>
#include <iostream>
#include <algorithm> // Usamos std::max
#include <string>

using std::list;
using std::cout;
using std::max;
using std::string;
using std::unordered_set;
using std::unique_ptr;
using std::make_unique;

/**
 * @brief Inicializamos nuestro Árbol AVL.
 * Comenzamos con nuestra raíz apuntando a la nada (nullptr) hasta que insertemos el primer elemento.
 */
Arbol_AVL::Arbol_AVL() : _raiz(nullptr) {}

/**
 * @brief El árbol se limpia solo al destruir la raíz (unique_ptr).
 * Al usar unique_ptr, no necesitamos borrar recursivamente a mano con 'delete'.

 * 'default' se encarga de llamar al destructor de cada unique_ptr, que a su vez llama al siguiente...
 * hasta que todos los nodos y sus listas se liberen automáticamente.
 */
Arbol_AVL::~Arbol_AVL() = default;

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
 * Obtenemos la diferencia entre nuestras ramas izquierda y derecha para saber hacia dónde inclinar el balanceo.
 * Si el resultado es >1 o <-1, el árbol está desequilibrado.
 * @param nodo_consulta Puntero al nodo cuyo balanceo queremos calcular.
 * @return Un entero que indica el grado/factor de desequilibrio (positivo: pesado a la izquierda, negativo: pesado a la derecha).
 */
int Arbol_AVL::obtener_balanceo(Nodo* nodo_consulta) {

    // Tratamos el caso base de un nodo nulo
    if (nodo_consulta == nullptr) {
        return 0;
    }
    // Usamos .get() para acceder a los hijos sin reclamar propiedad
    return (obtener_altura(nodo_consulta->_hijoIzquierdo.get()) - obtener_altura(nodo_consulta->_hijoDerecho.get()));
}

// === ROTACIONES DEL ÁRBOL (Transferencia de propiedad) ===

/**
 * @brief Realizamos un giro a la derecha en nuestro árbol.
 * Este movimiento nos permite rebalancear la estructura cuando nuestra rama izquierda pesa demasiado.
 * @param nodo_raiz_local Raíz del subárbol que ha perdido el equilibrio.
 * @return La nueva raíz local tras la rotación.
 */
unique_ptr<Nodo> Arbol_AVL::giro_derecha(unique_ptr<Nodo> nodo_raiz_local) {

    // Reclamamos la propiedad del hijo izquierdo
    unique_ptr<Nodo> nuevo_root = std::move(nodo_raiz_local->_hijoIzquierdo); 

    // Reclamamos la propiedad del hijo derecho del nuevo root
    unique_ptr<Nodo> sub_arbol_derecho = std::move(nuevo_root->_hijoDerecho); 

    // Reorganizamos punteros usando std::move
    nodo_raiz_local->_hijoIzquierdo = std::move(sub_arbol_derecho);
    
    // Actualizamos altura del antiguo root
    nodo_raiz_local->_altura = max(obtener_altura(nodo_raiz_local->_hijoIzquierdo.get()), 
                                   obtener_altura(nodo_raiz_local->_hijoDerecho.get())) + 1;

    // El antiguo root se convierte en el hijo derecho del nuevo root
    nuevo_root->_hijoDerecho = std::move(nodo_raiz_local);
    
    // Actualizamos altura del nuevo root
    nuevo_root->_altura = max(obtener_altura(nuevo_root->_hijoIzquierdo.get()), 
                              obtener_altura(nuevo_root->_hijoDerecho.get())) + 1;

    return nuevo_root; 
}

/**
 * @brief Ejecutamos un giro a la izquierda.
 * @param nodo_raiz_local Raíz del subárbol que ha perdido el equilibrio.
 * @return La nueva raíz local de la rama después del giro.
 */
unique_ptr<Nodo> Arbol_AVL::giro_izquierda(unique_ptr<Nodo> nodo_raiz_local) {
    unique_ptr<Nodo> nuevo_root = std::move(nodo_raiz_local->_hijoDerecho); 
    unique_ptr<Nodo> sub_arbol_izquierdo = std::move(nuevo_root->_hijoIzquierdo); 

    nodo_raiz_local->_hijoDerecho = std::move(sub_arbol_izquierdo);
    
    // Actualizamos altura del antiguo root
    nodo_raiz_local->_altura = max(obtener_altura(nodo_raiz_local->_hijoIzquierdo.get()), 
                                   obtener_altura(nodo_raiz_local->_hijoDerecho.get())) + 1;

    nuevo_root->_hijoIzquierdo = std::move(nodo_raiz_local);
    
    // Actualizamos altura del nuevo root
    nuevo_root->_altura = max(obtener_altura(nuevo_root->_hijoIzquierdo.get()), 
                              obtener_altura(nuevo_root->_hijoDerecho.get())) + 1;

    return nuevo_root; 
}

// === INSERCIÓN RECURSIVA ===

/**
 * @brief Método inicializador de inserción de un nuevo cuac en el árbol.
 */
void Arbol_AVL::insertar(Cuac* cuac_a_insertar) {
    // Transferimos la propiedad de la raíz para la recursión y la recuperamos al terminar
    _raiz = insertar_recursivo(std::move(_raiz), cuac_a_insertar);
}

/**
 * @brief Implementamos nuestra lógica de inserción balanceada.
 * @param nodo_actual Propietario actual del segmento del árbol.
 * @param nuevo_cuac Cuac a insertar.
 * @return El nuevo propietario del segmento del árbol.
 */
unique_ptr<Nodo> Arbol_AVL::insertar_recursivo(unique_ptr<Nodo> nodo_actual, Cuac* nuevo_cuac) {

    // CASO BASE: si llegamos a un hueco vacío
    if (nodo_actual == nullptr) {
        return make_unique<Nodo>(nuevo_cuac); 
    }

    const Fecha& fecha_nuevo = nuevo_cuac->get_fecha();

    // Navegamos transfiriendo la propiedad con std::move()
    if (fecha_nuevo < nodo_actual->_fecha) {
        nodo_actual->_hijoIzquierdo = insertar_recursivo(std::move(nodo_actual->_hijoIzquierdo), nuevo_cuac); 
    } else if (nodo_actual->_fecha < fecha_nuevo) {
        nodo_actual->_hijoDerecho = insertar_recursivo(std::move(nodo_actual->_hijoDerecho), nuevo_cuac); 
    } else {
        // Coincidencia de fecha: insertamos ordenadamente en la lista del nodo
        for (list<Cuac*>::iterator it_lista = nodo_actual->_listaCuacs.begin(); it_lista != nodo_actual->_listaCuacs.end(); ++it_lista) {
            Cuac* cuac_existente = *it_lista;
            if (nuevo_cuac->get_texto() < cuac_existente->get_texto()) {
                nodo_actual->_listaCuacs.insert(it_lista, nuevo_cuac);
                return nodo_actual; // Devolvemos la propiedad (no cambió el root)
            } else if (nuevo_cuac->get_texto() == cuac_existente->get_texto()) {
                if (nuevo_cuac->get_usuario() < cuac_existente->get_usuario()) {
                    nodo_actual->_listaCuacs.insert(it_lista, nuevo_cuac);
                    return nodo_actual;
                }
            }
        }
        nodo_actual->_listaCuacs.push_back(nuevo_cuac); 
        return nodo_actual; 
    }

    // Actualizamos la altura del nodo actual
    nodo_actual->_altura = max(obtener_altura(nodo_actual->_hijoIzquierdo.get()), 
                               obtener_altura(nodo_actual->_hijoDerecho.get())) + 1;

    // Comprobamos el factor de equilibrio
    int balanceo = obtener_balanceo(nodo_actual.get());

    // --- MANEJO DE DESBALANCEOS (ROTACIONES) ---

    // Izquierda pesada
    if (balanceo > 1) {
        if (fecha_nuevo < nodo_actual->_hijoIzquierdo->_fecha) {
            return giro_derecha(std::move(nodo_actual));
        } else {
            nodo_actual->_hijoIzquierdo = giro_izquierda(std::move(nodo_actual->_hijoIzquierdo));
            return giro_derecha(std::move(nodo_actual));
        }
    }

    // Derecha pesada
    if (balanceo < -1) {
        if (nodo_actual->_hijoDerecho->_fecha < fecha_nuevo) {
            return giro_izquierda(std::move(nodo_actual));
        } else {
            nodo_actual->_hijoDerecho = giro_derecha(std::move(nodo_actual->_hijoDerecho));
            return giro_izquierda(std::move(nodo_actual));
        }
    }

    return nodo_actual; 
}

// === BÚSQUEDAS (Solo lectura: usan punteros crudos .get()) ===

void Arbol_AVL::last(int cantidad_a_mostrar) {
    int restantes = cantidad_a_mostrar;
    int contador_posicion = 1;
    buscar_ultimos_recursivo(_raiz.get(), restantes, contador_posicion);
    cout << "Total: " << cantidad_a_mostrar - max(0, restantes) << " cuac" << "\n";
}

void Arbol_AVL::buscar_ultimos_recursivo(Nodo* nodo_actual, int& cuacs_restantes, int& contador_posicion) {
    if (nodo_actual == nullptr || cuacs_restantes <= 0) return;

    buscar_ultimos_recursivo(nodo_actual->_hijoDerecho.get(), cuacs_restantes, contador_posicion);

    for (Cuac* cuac : nodo_actual->_listaCuacs) {
        if (cuacs_restantes <= 0) break;
        cout << contador_posicion << ". ";
        cuac->write_cuac();
        cout << "\n";
        contador_posicion++; cuacs_restantes--;
    }

    if (cuacs_restantes > 0) {
        buscar_ultimos_recursivo(nodo_actual->_hijoIzquierdo.get(), cuacs_restantes, contador_posicion);
    }
}

void Arbol_AVL::date(const Fecha& fecha_inicio, const Fecha& fecha_fin) {
    int contador_total = 0;
    buscar_por_rango_recursivo(_raiz.get(), fecha_inicio, fecha_fin, contador_total);
    cout << "Total: " << contador_total << " cuac" << "\n";
}

void Arbol_AVL::buscar_por_rango_recursivo(Nodo* nodo_actual, const Fecha& fecha_inicio, const Fecha& fecha_fin, int& contador_total) {
    if (nodo_actual == nullptr) return;

    if (nodo_actual->_fecha <= fecha_fin) {
        buscar_por_rango_recursivo(nodo_actual->_hijoDerecho.get(), fecha_inicio, fecha_fin, contador_total);
    }

    if (nodo_actual->_fecha >= fecha_inicio && nodo_actual->_fecha <= fecha_fin) {
        for (Cuac* cuac : nodo_actual->_listaCuacs) {
            contador_total++;
            cout << contador_total << ". ";
            cuac->write_cuac();
            cout << "\n";
        }
    }

    if (nodo_actual->_fecha >= fecha_inicio) {
        buscar_por_rango_recursivo(nodo_actual->_hijoIzquierdo.get(), fecha_inicio, fecha_fin, contador_total);
    }
}

void Arbol_AVL::search(const string& texto) {
    int contador_total = 0;
    buscar_texto_recursivo(_raiz.get(), texto, contador_total);
    cout << "Total: " << contador_total << " cuac" << "\n";
}

void Arbol_AVL::buscar_texto_recursivo(Nodo* nodo_actual, const string& texto, int& contador_total) {
    if (nodo_actual == nullptr) return;

    buscar_texto_recursivo(nodo_actual->_hijoDerecho.get(), texto, contador_total);

    for (Cuac* cuac : nodo_actual->_listaCuacs) {
        if (cuac->get_texto().find(texto) != string::npos) {
            contador_total++;
            cout << contador_total << ". ";
            cuac->write_cuac();
            cout << "\n";
        }
    }

    buscar_texto_recursivo(nodo_actual->_hijoIzquierdo.get(), texto, contador_total);
}

void Arbol_AVL::lastFiltrado(int cantidad_a_mostrar, const unordered_set<string>& usuarios_permitidos) {
    int restantes = cantidad_a_mostrar;
    int contador_posicion = 1;
    buscar_ultimos_filtrado_recursivo(_raiz.get(), restantes, contador_posicion, usuarios_permitidos);
    cout << "Total: " << cantidad_a_mostrar - max(0, restantes) << " cuac" << "\n";
}

void Arbol_AVL::buscar_ultimos_filtrado_recursivo(Nodo* nodo_actual, int& cuacs_restantes,
    int& contador_posicion, const unordered_set<string>& usuarios_permitidos) {

    if (nodo_actual == nullptr || cuacs_restantes <= 0) return;

    buscar_ultimos_filtrado_recursivo(nodo_actual->_hijoDerecho.get(), cuacs_restantes,
        contador_posicion, usuarios_permitidos);

    for (Cuac* cuac : nodo_actual->_listaCuacs) {
        if (cuacs_restantes <= 0) break;
        if (usuarios_permitidos.count(cuac->get_usuario()) > 0) {
            cout << contador_posicion << ". ";
            cuac->write_cuac();
            cout << "\n";
            contador_posicion++; cuacs_restantes--;
        }
    }

    if (cuacs_restantes > 0) {
        buscar_ultimos_filtrado_recursivo(nodo_actual->_hijoIzquierdo.get(), cuacs_restantes,
            contador_posicion, usuarios_permitidos);
    }
}

// === ELIMINACIÓN (RAII) ===

void Arbol_AVL::eliminar(int id_cuac, const Fecha& fecha_cuac) {
    _raiz = eliminar_recursivo(std::move(_raiz), id_cuac, fecha_cuac);
}

/**
 * @brief Obtiene el nodo con el valor mínimo (sucesor inorden).
 * @return Puntero crudo al nodo (uso temporal de consulta).
 */
Nodo* Arbol_AVL::obtener_nodo_minimo(Nodo* nodo_actual) {
    Nodo* actual = nodo_actual;
    while (actual->_hijoIzquierdo != nullptr) {
        actual = actual->_hijoIzquierdo.get();
    }
    return actual;
}

/**
 * @brief Lógica recursiva para eliminar un cuac.
 */
unique_ptr<Nodo> Arbol_AVL::eliminar_recursivo(unique_ptr<Nodo> nodo_actual, int id_cuac, const Fecha& fecha_cuac) {

    if (nodo_actual == nullptr) return nullptr;

    if (fecha_cuac < nodo_actual->_fecha) {
        nodo_actual->_hijoIzquierdo = eliminar_recursivo(std::move(nodo_actual->_hijoIzquierdo), id_cuac, fecha_cuac);
    } else if (fecha_cuac > nodo_actual->_fecha) {
        nodo_actual->_hijoDerecho = eliminar_recursivo(std::move(nodo_actual->_hijoDerecho), id_cuac, fecha_cuac);
    } else {
        // Encontrado: buscamos el ID específico en la lista
        if (id_cuac != -1) {
            for (auto it = nodo_actual->_listaCuacs.begin(); it != nodo_actual->_listaCuacs.end(); ++it) {
                if ((*it)->get_id() == id_cuac) {
                    nodo_actual->_listaCuacs.erase(it);
                    break;
                }
            }
        }

        // Si el nodo quedó vacío, procedemos a borrar el nodo físico
        if (nodo_actual->_listaCuacs.empty()) {
            
            // Caso 0 o 1 hijos
            if (nodo_actual->_hijoIzquierdo == nullptr) return std::move(nodo_actual->_hijoDerecho);
            if (nodo_actual->_hijoDerecho == nullptr) return std::move(nodo_actual->_hijoIzquierdo);

            // Caso 2 hijos: sucesor inorden
            Nodo* temp = obtener_nodo_minimo(nodo_actual->_hijoDerecho.get());
            
            nodo_actual->_fecha = temp->_fecha;
            nodo_actual->_listaCuacs = temp->_listaCuacs;

            // Borramos el sucesor
            nodo_actual->_hijoDerecho = eliminar_recursivo(std::move(nodo_actual->_hijoDerecho), -1, temp->_fecha);
        } else {
            return nodo_actual; 
        }
    }

    if (nodo_actual == nullptr) return nullptr;

    // Actualización de altura
    nodo_actual->_altura = max(obtener_altura(nodo_actual->_hijoIzquierdo.get()), 
                               obtener_altura(nodo_actual->_hijoDerecho.get())) + 1;

    int balance = obtener_balanceo(nodo_actual.get());

    // Re-balanceo tras eliminación
    if (balance > 1) {
        if (obtener_balanceo(nodo_actual->_hijoIzquierdo.get()) >= 0) {
            return giro_derecha(std::move(nodo_actual));
        } else {
            nodo_actual->_hijoIzquierdo = giro_izquierda(std::move(nodo_actual->_hijoIzquierdo));
            return giro_derecha(std::move(nodo_actual));
        }
    }

    if (balance < -1) {
        if (obtener_balanceo(nodo_actual->_hijoDerecho.get()) <= 0) {
            return giro_izquierda(std::move(nodo_actual));
        } else {
            nodo_actual->_hijoDerecho = giro_derecha(std::move(nodo_actual->_hijoDerecho));
            return giro_izquierda(std::move(nodo_actual));
        }
    }

    return nodo_actual;
}

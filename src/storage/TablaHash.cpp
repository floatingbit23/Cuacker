#include <iostream>
#include <list>
#include <string>
#include "Fecha.h"
#include "Cuac.h"
#include "TablaHash.h"

/**
 * @brief Inicializamos nuestra Tabla Hash.
 * Empezamos con un tamaño pequeño (101) que irá creciendo dinámicamente.
 */
TablaHash::TablaHash() {
    _num_buckets = 101; // Tamaño inicial de la tabla
    _tabla_buckets = new std::list<Par>[_num_buckets]; 
    _num_elementos = 0; // Contador de elementos
    _num_usuarios = 0; // Contador de usuarios únicos (Usado para el rehash dinámico)
}

/**
 * @brief Liberamos la memoria dinámica que hemos solicitado.
 * Nos aseguramos de borrar el array de listas para liberar RAM y evitar fugas de memoria (Memory Leaks).
 */
TablaHash::~TablaHash() {
    delete[] _tabla_buckets;
}

/**
 * @brief Implementamos nuestra función de dispersión (Hash).
 * Aplicamos un algoritmo polinomial simple sobre la cadena del nombre de usuario
 * para obtener un índice dentro del rango de nuestra tabla.
 * @param clave La cadena de texto (nombre de usuario) que queremos dispersar.
 * @return El índice calculado (0 a _num_buckets-1).
 */
int TablaHash::funcionHash(const std::string& clave)
{
    unsigned long suma = 0;
    for (unsigned char c : clave) {
        // Multiplicamos por 31 (un primo común en funciones hash) y sumamos el caracter
        suma = (suma * 31) + c;
    }
    return suma % _num_buckets; // Aplicamos el módulo para ajustarnos al tamaño B
}

/**
 * @brief Proceso de inserción en nuestra estructura.
 * En nuestro diseño, insertamos el cuac manteniendo el orden dentro de la lista de cada usuario.
 * @param c El objeto Cuac que queremos almacenar.
 * @return La dirección de memoria donde ha quedado guardado el cuac insertado.
 */
Cuac* TablaHash::insertar(const Cuac& c, bool* esNuevo)
{
    std::string usuario = c.get_usuario();
    int h = funcionHash(usuario); // Calculamos en qué "cubo/bucket" debe estar

    // Por defecto, asumimos que no es nuevo a menos que lleguemos al final
    if (esNuevo) *esNuevo = false;

    // Buscamos si el usuario ya tiene una entrada en nuestra tabla
    for (Par &par : _tabla_buckets[h]) {

        // Si encontramos al usuario, insertamos el cuac en su lista

        // == CASO DE USUARIO EXISTENTE EN EL SISTEMA ==
        
        if (par._nombre_usuario == usuario) {
            const Fecha& fecha = c.get_fecha(); 
            std::string texto = c.get_texto();

            std::list<Cuac>::iterator itera_Cuac = par._lista_cuacs.begin(); // Objeto iterador para recorrer la lista

            // Buscamos la posición correcta para mantener la lista ordenada (por fecha, texto y usuario)
            while (itera_Cuac != par._lista_cuacs.end()) {
                const Fecha& fecha2 = itera_Cuac->get_fecha();

                if (fecha < fecha2) {
                    ++itera_Cuac;
                    continue;

                } else if (fecha == fecha2){

                    // Si coinciden en fecha, comparamos el texto
                    std::string texto2 = itera_Cuac->get_texto();

                    if (texto > texto2) {
                        ++itera_Cuac;
                        continue;

                    } else if (texto == texto2) {

                        // Si incluso el texto coincide, desempatamos por nombre de usuario
                        std::string usuario2 = itera_Cuac->get_usuario();

                        if (usuario > usuario2) {
                            ++itera_Cuac;
                            continue;
                        }
                    }
                }
                break;
            } 

            // Insertamos en la posición que hemos determinado y devolvemos su dirección
            std::list<Cuac>::iterator dir_cuac = par._lista_cuacs.insert(itera_Cuac, c);
            _num_elementos++; // Incrementamos el contador de elementos

            return &(*dir_cuac); 
            // 1. Desreferenciamos el iterador para obtener el objeto Cuac (*dir_cuac)
            // 2. Obtenemos la dirección de memoria de ese objeto (&)
        }
    }

    // == CASO DE USUARIO NUEVO EN EL SISTEMA==

    Par nuevo; // Creamos un nuevo par
    nuevo._nombre_usuario = usuario; // Le asignamos el nombre del usuario
    nuevo._lista_cuacs.push_back(c); // Le asignamos el cuac (push_back inserta al final)
    _tabla_buckets[h].push_back(nuevo); // Lo insertamos en la tabla (al final)
    
    if (esNuevo) *esNuevo = true; // Si es un usuario nuevo, lo marcamos como tal

    // Incrementamos el contador de elementos y usuarios
    _num_elementos++; 
    _num_usuarios++; 

    // Guardamos la dirección ANTES del rehash, porque si hay rehash la dirección cambiará
    Cuac* direccion = &(_tabla_buckets[h].back()._lista_cuacs.back());

    // Comprobamos el factor de carga (Load Factor) e incrementamos si es necesario (>0.75)
    if ((float)_num_usuarios / _num_buckets > 0.75f) {

        rehash(); // Duplica el tamaño de la tabla y redistribuye los elementos

        /*
        NOTA: No hace falta recalcular 'direcciones de los cuacs' aquí.
        Como usamos std::list y std::move() en el método rehash(), tras ejecutarlo 
        los objetos Cuac  permanecerán en la misma posición de memoria ("Estabilidad de Memoria").
        Los punteros del AVL y del Diccionario seguirán siendo válidos.
        Evita errores de Crash/Segmentation Fault!
        */
    }

    return direccion; // Devolvemos la dirección del último cuac insertado
}

/**
 * @brief Implementamos el comando "follow" de nuestra red social.
 * Buscamos la lista de cuacs de un usuario específico en nuestra tabla.
 * @param usuario Nombre del usuario seguido.
 * @return Una lista con todos los mensajes del usuario, ordenada cronológicamente.
 */
std::list<Cuac> TablaHash::follow(const std::string& usuario) {

    int h = funcionHash(usuario); // Calculamos en qué "cubo/bucket" debe estar

    // Recorremos la lista de pares 'Par(nombre, lista de cuacs)' en el cubo correspondiente
    for (Par &par : _tabla_buckets[h]) {
        if (par._nombre_usuario == usuario) { // Si encontramos al usuario
            return par._lista_cuacs; // Devolvemos su lista
        }
    }

    // Si no lo encontramos, devolvemos una lista vacía
    std::list<Cuac> vacia;
    return vacia;
}

/**
 * @brief Comprobamos si un usuario ya tiene alguna publicación en el sistema.
 * @param usuario Nombre del usuario a buscar.
 * @return true si el usuario ya existe, false en caso contrario.
 */
bool TablaHash::existeUsuario(const std::string& usuario) {

    int h = funcionHash(usuario); // Calculamos en qué "cubo/bucket" debe estar
    
    // Recorremos la lista de pares 'Par(nombre, lista de cuacs)' en el cubo correspondiente
    for (Par &par : _tabla_buckets[h]) {

        if (par._nombre_usuario == usuario) { // Si encontramos al usuario
            return true; // Devolvemos true
        }
        
    }

    return false; // Si no lo encontramos, devolvemos false
}


/**
 * @brief Elimina un Cuac específico conociendo a su autor (O(k_usuario))
 * @param usuario Nombre del autor del cuac a borrar.
 * @param id_cuac ID del cuac a borrar.
 * @return true si se eliminó, false en caso contrario.
 */
bool TablaHash::eliminar(const std::string& usuario, int id_cuac) {

    int h = funcionHash(usuario); // Calculamos en qué "cubo/bucket" debe estar

    // Recorremos la lista de pares 'Par(nombre, lista de cuacs)' en el cubo correspondiente
    for (Par& par : _tabla_buckets[h]) {

        if (par._nombre_usuario == usuario) { // Si encontramos al usuario

            // Recorremos la lista de cuacs del usuario (O(k))
            for (auto it = par._lista_cuacs.begin(); it != par._lista_cuacs.end(); ++it) {
                
                if (it->get_id() == id_cuac) { // Si encontramos el Cuac
                    
                    par._lista_cuacs.erase(it); // Eliminamos el Cuac
                    _num_elementos--; // Decrementamos el contador de elementos

                    return true; // Devolvemos true
                }
            }

            break; // Salimos del bucle (solo puede haber un 'Par' por usuario)
        }
    }
    return false; // Si no lo encontramos, devolvemos false
}


/**
 * @brief Función de rehash dinámico
 * Redimensiona la tabla cuando el factor de carga es alto (>0.75) para mantener el coste O(1).
 */
void TablaHash::rehash() {

    // Guardamos el número de buckets y la tabla actual
    int old_num_buckets = _num_buckets;
    std::list<Par>* old_buckets = _tabla_buckets;

    // Duplicamos el tamaño de la tabla (buscando un número impar para evitar colisiones)
    _num_buckets = (_num_buckets * 2) + 1;
    _tabla_buckets = new std::list<Par>[_num_buckets]; // Asignamos memoria para la nueva tabla

    // Reasignamos todos los pares a los nuevos buckets
    for (int i = 0; i < old_num_buckets; ++i) {

        for (Par& par : old_buckets[i]) { // Recorremos la lista de pares en cada bucket

            // Calculamos el hash del usuario ya existente para moverlo a su nuevo bucket
            int h = funcionHash(par._nombre_usuario);

            /* 
            std::move permite mover elementos de una lista a otra sin reasignar memoria!
            Esto es CRÍTICO porque garantiza que: 
            1. C++ no copiará los elemeentos de la lista uno a uno
            2. Simplemente "desengancha" la lista del sitio viejo y la "engancha" en el nuevo bucket de la tabla
            3. Los punteros internos de los objetos 'Cuac' no se rompen y los mensajes no se mueven de su sitio en la RAM
            */
            _tabla_buckets[h].push_back(std::move(par));
        }
    }

    delete[] old_buckets; // Borramos el array viejo
}

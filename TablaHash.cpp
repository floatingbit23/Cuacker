#include <iostream>
#include <list>
#include <string>
#include "Fecha.h"
#include "Cuac.h"
#include "TablaHash.h"

/**
 * @brief Inicializamos nuestra Tabla Hash.
 * Elegimos un número primo grande (300007) para el tamaño de nuestra tabla para minimizar las colisiones y maximizar la eficiencia.
 */
TablaHash::TablaHash() {
    B = 300007;
    T = new std::list<Par>[B]; // Reservamos el espacio en memoria dinámica
    n = 0; // Inicializamos nuestro contador de elementos
}

/**
 * @brief Liberamos la memoria dinámica que hemos solicitado.
 * Nos aseguramos de borrar el array de listas para liberar RAM y evitar fugas de memoria (Memory Leaks).
 */
TablaHash::~TablaHash() {
    delete[] T;
}

/**
 * @brief Implementamos nuestra función de dispersión (Hash).
 * Aplicamos un algoritmo polinomial simple sobre la cadena del nombre de usuario
 * para obtener un índice dentro del rango de nuestra tabla.
 * @return El índice calculado (0 a B-1).
 */
int TablaHash::funcionHash(std::string& clave)
{
    unsigned long suma = 0;
    for (unsigned char c : clave) {
        // Multiplicamos por 31 (un primo común en funciones hash) y sumamos el caracter
        suma = (suma * 31) + c;
    }
    return suma % B; // Aplicamos el módulo para ajustarnos al tamaño B
}

/**
 * @brief Proceso de inserción en nuestra estructura.
 * En nuestro diseño, insertamos el cuac manteniendo el orden dentro de la lista de cada usuario.
 * @return La dirección de memoria donde ha quedado guardado el cuac insertado.
 */
Cuac* TablaHash::insertar(Cuac c)
{
    std::string usuario = c.get_usuario();
    int h = funcionHash(usuario); // Calculamos en qué "cubo" debe estar

    // Buscamos si el usuario ya tiene una entrada en nuestra tabla
    for (Par &par : T[h]) {

        // Si encontramos al usuario, insertamos el cuac en su lista
        if (par.nombre == usuario) {
            Fecha fecha = c.get_fecha(); 
            std::string texto = c.get_texto();

            std::list<Cuac>::iterator itera_Cuac = par.l.begin(); // Objeto iterador para recorrer la lista

            // Buscamos la posición correcta para mantener la lista ordenada (por fecha, texto y usuario)
            while (itera_Cuac != par.l.end()) {
                Fecha fecha2 = itera_Cuac->get_fecha();

                if (fecha.es_menor(fecha2)) {
                    ++itera_Cuac;
                    continue;

                } else if (fecha.es_igual(fecha2)){

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
            std::list<Cuac>::iterator dir_cuac = par.l.insert(itera_Cuac, c);
            n++; // Incrementamos el contador de elementos

            return &(*dir_cuac); 
            // 1. Desreferenciamos el iterador para obtener el objeto Cuac (*dir_cuac)
            // 2. Obtenemos la dirección de memoria de ese objeto (&)
        }
    }

    // Si llegamos aquí, es que es un usuario nuevo en nuestro sistema:

    Par nuevo; // Creamos un nuevo par
    nuevo.nombre = usuario; // Le asignamos el nombre del usuario
    nuevo.l.push_back(c); // Le asignamos el cuac (push_back inserta al final)
    T[h].push_back(nuevo); // Lo insertamos en la tabla (al final)
    
    n++; // Incrementamos el contador de elementos

    return &(T[h].back().l.back()); // Devolvemos la dirección del elemento recién creado
}

/**
 * @brief Implementamos el comando "follow" de nuestra red social.
 * Buscamos la lista de cuacs de un usuario específico en nuestra tabla.
 * @return Una lista con todos los mensajes del usuario, ordenada cronológicamente.
 */
std::list<Cuac> TablaHash::follow(std::string& usuario) {

    // Comunicamos la acción que estamos realizando
    std::cout << "follow " << usuario << std::endl;

    int h = funcionHash(usuario); // Calculamos en qué "cubo" debe estar

    // Recorremos la lista de pares 'Par(nombre, lista de cuacs)' en el cubo correspondiente
    for (Par &par : T[h]) {
        if (par.nombre == usuario) { // Si encontramos al usuario
            return par.l; // Devolvemos su lista
        }
    }

    // Si no lo encontramos, devolvemos una lista vacía
    std::list<Cuac> vacia;
    return vacia;
}



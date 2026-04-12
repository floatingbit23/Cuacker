#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include "DiccionarioCuacs.h" 
#include "Persistencia.h" // Necesario para poder usar el puntero a Persistencia y llamar a sus métodos (insertar, eliminar, etc.)

/**
 * @brief Constructor de nuestra clase Diccionario
 * Inicializamos el contador de usuarios únicos a cero.
 */
DiccionarioCuacs::DiccionarioCuacs(){
    _num_usuarios_unicos = 0; // Inicializamos el contador de usuarios únicos a cero
    _persistencia = nullptr;  // Sin persistencia hasta que se inyecte (evita INSERTs durante la carga)
}

/**
 * @brief Extraemos los hashtags del texto de un cuac y los indexamos.
 * Recorremos el texto palabra por palabra buscando tokens que empiezan con '#'.
 * Almacenamos cada hashtag (sin el '#') como clave en nuestro mapa de índices.
 * @param texto Texto del cuac del que extraer los hashtags.
 * @param cuac_ptr Puntero al cuac que contiene los hashtags.
 */
void DiccionarioCuacs::extraer_hashtags(const std::string& texto, Cuac* cuac_ptr) {

    size_t pos = 0;
    
    // Buscamos el carácter '#' directamente en la cadena
    while ((pos = texto.find('#', pos)) != std::string::npos) {
        
        // El hashtag termina en el siguiente espacio, salto de línea o al final del texto
        size_t fin = texto.find_first_of(" \n\t\r", pos);
        
        // Extraemos la palabra (sin el '#')
        std::string hashtag = texto.substr(pos + 1, fin - pos - 1);
        
        // Si el hashtag es válido, lo indexamos
        if (!hashtag.empty()) {
            _indice_hashtags[hashtag].push_back(cuac_ptr);
        }
        
        // Si no hay más texto, salimos; si hay, seguimos buscando desde el final del hashtag actual
        if (fin == std::string::npos) break;
        pos = fin;
    }
}

/**
 * @brief Implementamos el proceso de inserción dual.
 * Primero guardamos el cuac en nuestra Tabla Hash (que es la dueña del objeto)
 * y usamos el puntero devuelto para indexarlo en nuestro Árbol AVL.
 * Además, extraemos los hashtags del texto para indexarlos en nuestro mapa.
 * @param nuevo Referencia constante al objeto Cuac que queremos insertar.
 */
void DiccionarioCuacs::insertar(const Cuac &nuevo){

    bool es_nuevo_usuario = false;

    // 1. Guardamos el cuac en la TablaHash y detectamos si el usuario es nuevo en una sola operación
    // Esto nos ahorra calcular el Hash y recorrer los buckets dos veces.
    // Obtenemos el puntero al cuac insertado para usarlo en el árbol AVL y en el índice de hashtags
    Cuac* pt = _tabla_usuarios.insertar(nuevo, &es_nuevo_usuario); 
    
    // Si la inserción fue exitosa (pt no es nulo), procedemos a indexarlo
    if (pt != nullptr) {

        // Actualizamos el contador de usuarios globales si detectamos que es uno nuevo
        if (es_nuevo_usuario) {
            _num_usuarios_unicos++;
        }

        // 2. Insertamos el puntero en el árbol AVL para mantener el orden cronológico
        _arbol_fechas.insertar(pt);

        // Indexamos el puntero (con el texto del cuac) en el mapa de hashtags (_indice_hashtags)
        // 3. Extraemos los hashtags (usando el nuevo optimizador de texto)
        extraer_hashtags(pt->get_texto(), pt);

        // 4. Registramos el ID en el índice secundario para acceso instantáneo
        _indice_ids[pt->get_id()] = pt;

        // === GUARDADO EN BASE DE DATOS ===

        // 5. Auto-guardado: si la persistencia está activa (no es nullptr), guardamos en SQLite inmediatamente
        if (_persistencia != nullptr) {
            _persistencia->insertar(*pt);
        }
    }
}

/**
 * @brief Elimina una publicación de todo el sistema por su ID.
 * Requiere coordinación entre Hash, AVL y el índice de Tags.
 */
void DiccionarioCuacs::eliminar(int id_cuac) {
    // 1. Buscamos el puntero en el índice de IDs (O(1))
    auto it_id = _indice_ids.find(id_cuac);
    
    if (it_id == _indice_ids.end()) {
        std::cout << "[!] Error: No se ha encontrado el Cuac con ID #" << id_cuac << std::endl;
        return;
    }

    Cuac* pt = it_id->second;
    std::string usuario = pt->get_usuario();
    Fecha fecha = pt->get_fecha();

    // 2. Borramos del Árbol AVL (O(log n))
    _arbol_fechas.eliminar(id_cuac, fecha);

    // 3. Borramos de la Tabla Hash principal (O(k_usuario))
    // Usamos el nuevo método optimizado que va directo al bucket del usuario
    _tabla_usuarios.eliminar(usuario, id_cuac);

    // 4. Borramos del índice de IDs (O(1))
    _indice_ids.erase(it_id);

    // === BORRADO EN BASE DE DATOS ===

    // 5. Auto-guardado: si la persistencia está activa (no es nullptr), eliminamos de SQLite inmediatamente
    if (_persistencia != nullptr) {
        _persistencia->eliminar(id_cuac);
    }

    std::cout << "[i] Cuac #" << id_cuac << " eliminado permanentemente del sistema." << std::endl;
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
void DiccionarioCuacs::follow(const std::string& nombre){

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

    std::cout << "Total: " << lista.size() << " cuac\n" << std::endl; // Ejemplo: "Total: 3 cuac"
}

/**
 * @brief Filtramos las publicaciones por rango de fechas.
 * Aprovechamos la estructura de nuestro árbol para encontrar eficientemente los nodos relevantes.
 * @param fecha Límite inferior del rango de búsqueda.
 * @param fecha2 Límite superior del rango de búsqueda.
 */
void DiccionarioCuacs::date(const Fecha& fecha, const Fecha& fecha2){ 
    // Ej. date 01/01/2022 31/12/2022
    _arbol_fechas.date(fecha, fecha2); 
}

/**
 * @brief Buscamos y mostramos los cuacs que contienen un hashtag específico.
 * Consultamos nuestro índice de hashtags para obtener la lista de cuacs asociados.
 * @param hashtag El hashtag a buscar (con o sin '#').
 */
void DiccionarioCuacs::tag(const std::string& hashtag) {

    // Limpiamos el '#' si el usuario lo ha incluido en la búsqueda

    std::string tag_limpio = hashtag;
    // Si el hashtag no está vacío y empieza con '#', lo eliminamos
    if (!tag_limpio.empty() && tag_limpio[0] == '#') {
        tag_limpio = tag_limpio.substr(1);
    }

    // Buscamos el hashtag en nuestro índice, usando un iterador para recorrer el mapa 
    // y el método find(), que nos devuelve un iterador al elemento encontrado o al final del mapa si no existe
    std::map<std::string, std::list<Cuac*>>::iterator it = _indice_hashtags.find(tag_limpio);

    // Si el iterador no es igual al final del mapa, significa que encontramos el hashtag
    if (it != _indice_hashtags.end()) {

        // Encontramos el hashtag: mostramos todos los cuacs que lo contienen
        int contador = 1;

        // Recorremos la lista de punteros a cuacs asociados al hashtag
        for (Cuac* cuac : it->second) {
            std::cout << contador << ". ";
            cuac->write_cuac();
            std::cout << "\n";
            contador++; // Incrementamos el contador para el siguiente cuac
        }

        std::cout << "Total: " << it->second.size() << " cuac\n" << std::endl;

    } else {
        // No encontramos ningún cuac con ese hashtag
        std::cout << "Total: 0 cuac\n" << std::endl;
    }
}

/**
 * @brief Buscamos cuacs cuyo texto contenga una subcadena específica.
 * Delegamos la búsqueda en nuestro Árbol AVL, que realiza un recorrido inorden (izquierda -> raíz -> derecha) filtrando por el contenido del texto.
 * @param texto Subcadena a buscar dentro del contenido de los cuacs.
 */
void DiccionarioCuacs::search(const std::string& texto) {

    // Delegamos la búsqueda en nuestro árbol
    _arbol_fechas.search(texto);
}

/**
 * @brief Mostramos estadísticas generales de nuestra red social Cuacker.
 * Incluye:
 *  - Total de cuacs almacenados
 *  - Hashtags únicos registrados
 *  - Número total de hashtags encontrados en todos los cuacs.
 */
void DiccionarioCuacs::stats() {
    std::cout << "\n=== Estadisticas de Cuacker ===" << std::endl;
    std::cout << "Total cuacs: " << _tabla_usuarios.nElem() << std::endl;
    std::cout << "Usuarios unicos: " << _num_usuarios_unicos << std::endl;
    std::cout << "Hashtags unicos: " << _indice_hashtags.size() << std::endl;

    // Contamos el total de asociaciones hashtag-cuac
    int total_asociaciones = 0;
    for (std::map<std::string, std::list<Cuac*>>::iterator it = _indice_hashtags.begin(); 
         it != _indice_hashtags.end(); ++it) {
        total_asociaciones += it->second.size();
    }
    std::cout << "Total asociaciones tag-cuac: " << total_asociaciones << std::endl;
    std::cout << "===============================\n" << std::endl;
}

/**
 * @brief Setter: Inyecta la referencia a la capa de persistencia SQLite.
 * Se llama DESPUÉS de cargar los datos iniciales para activar el auto-guardado.
 * @param p Puntero a la instancia de Persistencia.
 */
void DiccionarioCuacs::setPersistencia(Persistencia* p) {
    _persistencia = p;
}

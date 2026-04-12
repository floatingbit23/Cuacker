#include "Persistencia.h"
#include "sqlite3.h"
#include <iostream>

/**
 * @file Persistencia.cpp
 * @brief Implementación de la capa de persistencia SQLite para Cuacker.
 * Gestiona la conexión, esquema, y operaciones incrementales sobre la BBDD.
 */

/**
 * @brief Constructor: abre la conexión SQLite y prepara los statements.
 * @param ruta_db Ruta al fichero .db (se crea automáticamente si no existe).
 */
Persistencia::Persistencia(const std::string& ruta_db) 
    : _db(nullptr), _stmt_insertar(nullptr), _stmt_eliminar(nullptr),
      _stmt_seguir(nullptr), _stmt_dejar(nullptr), _ruta_db(ruta_db) {

    // _db es un puntero a sqlite3, que es la estructura que representa la conexión a la BBDD,
    //  se inicializa a nullptr porque no hay conexión al principio

    // _stmt_insertar y _stmt_eliminar son punteros a sqlite3_stmt, que representan los prepared statements
    // _ruta_db es la ruta al fichero .db
        
    // Abrimos la conexión a la base de datos SQLite (rc -> remote connection)
    int rc = sqlite3_open(ruta_db.c_str(), &_db);

    // Si la conexión no se pudo abrir, mostramos un error y salimos
    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error critico al abrir la base de datos: " << sqlite3_errmsg(_db) << std::endl;
        _db = nullptr;
        return;
    }

    // Activamos el modo WAL (Write-Ahead Logging) para mejorar el rendimiento concurrente
    sqlite3_exec(_db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);

    // Creamos la tabla y los índices si no existen
    crearEsquema();

    // Precompilamos los prepared statements para INSERT y DELETE
    const char* sql_insert = 
        "INSERT INTO cuacs (id, usuario, dia, mes, anio, hora, minuto, segundo, tipo_cuac, mensaje, numero_predefinido) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    const char* sql_delete = "DELETE FROM cuacs WHERE id = ?;";

    // Preparamos el statement para INSERT
    rc = sqlite3_prepare_v2(_db, sql_insert, -1, &_stmt_insertar, nullptr); 

    // Si la preparación del statement falla, mostramos un error y salimos
    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al preparar INSERT: " << sqlite3_errmsg(_db) << std::endl;
    }

    // Preparamos el statement para DELETE
    rc = sqlite3_prepare_v2(_db, sql_delete, -1, &_stmt_eliminar, nullptr);
    
    // Si la preparación del statement falla, mostramos un error y salimos
    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al preparar DELETE: " << sqlite3_errmsg(_db) << std::endl;
    }

    // Precompilamos los prepared statements para el GRAFO SOCIAL (seguidores)
    const char* sql_seguir = "INSERT OR IGNORE INTO seguidores (seguidor, seguido) VALUES (?, ?);";
    const char* sql_dejar  = "DELETE FROM seguidores WHERE seguidor = ? AND seguido = ?;";

    // Preparamos el statement para FOLLOW
    rc = sqlite3_prepare_v2(_db, sql_seguir, -1, &_stmt_seguir, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al preparar FOLLOW: " << sqlite3_errmsg(_db) << std::endl;
    }

    // Preparamos el statement para UNFOLLOW
    rc = sqlite3_prepare_v2(_db, sql_dejar, -1, &_stmt_dejar, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al preparar UNFOLLOW: " << sqlite3_errmsg(_db) << std::endl;
    }
}

/**
 * @brief Destructor: libera los Prepared Statements y cierra la conexión SQLite.
 * Se ejecuta automáticamente al salir del scope.
 */
Persistencia::~Persistencia() {
    if (_stmt_insertar) sqlite3_finalize(_stmt_insertar); // Libera el prepared statement de INSERT
    if (_stmt_eliminar) sqlite3_finalize(_stmt_eliminar); // Libera el prepared statement de DELETE
    if (_stmt_seguir)   sqlite3_finalize(_stmt_seguir);   // Libera el prepared statement de FOLLOW
    if (_stmt_dejar)    sqlite3_finalize(_stmt_dejar);     // Libera el prepared statement de UNFOLLOW
    if (_db) sqlite3_close(_db); // Cierra la conexión a la base de datos
}

/**
 * @brief Crea el esquema de la base de datos (tabla + índices) si no existe.
 * Se ejecuta una sola vez al abrir la conexión por primera vez.
 */
void Persistencia::crearEsquema() {

    const char* sql_crear_tabla = 
        "CREATE TABLE IF NOT EXISTS cuacs ("
        "    id                  INTEGER PRIMARY KEY,"
        "    usuario             TEXT    NOT NULL,"
        "    dia                 INTEGER NOT NULL,"
        "    mes                 INTEGER NOT NULL,"
        "    anio                INTEGER NOT NULL,"
        "    hora                INTEGER NOT NULL,"
        "    minuto              INTEGER NOT NULL,"
        "    segundo             INTEGER NOT NULL,"
        "    tipo_cuac           TEXT    NOT NULL,"
        "    mensaje             TEXT    DEFAULT '',"
        "    numero_predefinido  INTEGER DEFAULT 0"
        ");";

    // Índice para el comando 'follow'
    const char* sql_indice_usuario = 
        "CREATE INDEX IF NOT EXISTS idx_cuacs_usuario ON cuacs(usuario);";

    // Índice compuesto para el comando 'date'
    const char* sql_indice_fecha = 
        "CREATE INDEX IF NOT EXISTS idx_cuacs_fecha ON cuacs(anio, mes, dia, hora, minuto, segundo);";

    char* err_msg = nullptr; // Puntero que almacenará el mensaje de error si ocurre alguno

    // Ejecutamos la creación de la tabla
    int rc = sqlite3_exec(_db, sql_crear_tabla, nullptr, nullptr, &err_msg);

    // Si la creación de la tabla falla, mostramos un error y salimos
    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al crear tabla: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }

    // Creamos el índice por usuario (para el comando 'follow')
    rc = sqlite3_exec(_db, sql_indice_usuario, nullptr, nullptr, &err_msg);

    // Si la creación del índice falla, mostramos un error y salimos
    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al crear indice usuario: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }

    // Creamos el índice compuesto por fecha (para el comando 'date')
    rc = sqlite3_exec(_db, sql_indice_fecha, nullptr, nullptr, &err_msg);
    
    // Si la creación del índice falla, mostramos un error y salimos
    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al crear indice fecha: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }

    // === TABLA DEL GRAFO SOCIAL ===

    const char* sql_crear_seguidores = 
        "CREATE TABLE IF NOT EXISTS seguidores ("
        "    seguidor    TEXT NOT NULL,"
        "    seguido     TEXT NOT NULL,"
        "    PRIMARY KEY (seguidor, seguido)"
        ");";

    // Índice para consultas inversas: "¿quién me sigue?"
    const char* sql_indice_seguido = 
        "CREATE INDEX IF NOT EXISTS idx_seguidores_seguido ON seguidores(seguido);";

    // Creamos la tabla de seguidores
    rc = sqlite3_exec(_db, sql_crear_seguidores, nullptr, nullptr, &err_msg);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al crear tabla seguidores: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }

    // Creamos el índice inverso de seguidores
    rc = sqlite3_exec(_db, sql_indice_seguido, nullptr, nullptr, &err_msg);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al crear indice seguidores: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }
}

/**
 * @brief Carga todos los cuacs almacenados en la base de datos SQLite.
 * Reconstruye los objetos Cuac usando el constructor con ID explícito.
 * También sincroniza el contador estático de IDs con el MAX(id) de la BBDD.
 * @return Lista de objetos Cuac reconstruidos.
 */
std::list<Cuac> Persistencia::cargar() {

    std::list<Cuac> cuacs;

    if (!_db) return cuacs;

    // Recuperamos el MAX(id) para sincronizar el contador autoincremental
    sqlite3_stmt* stmt_max = nullptr;
    int max_id = 0;

    int rc = sqlite3_prepare_v2(_db, "SELECT MAX(id) FROM cuacs;", -1, &stmt_max, nullptr);

    if (rc == SQLITE_OK && sqlite3_step(stmt_max) == SQLITE_ROW) {
        // sqlite3_column_int devuelve 0 si la tabla está vacía (NULL), que es correcto
        max_id = sqlite3_column_int(stmt_max, 0);
    }

    sqlite3_finalize(stmt_max);

    // Sincronizamos el contador de IDs: los nuevos cuacs empezarán desde max_id + 1
    Cuac::inicializar_contador(max_id + 1);

    // Ahora recuperamos todos los cuacs de la BBDD
    sqlite3_stmt* stmt_select = nullptr;
    rc = sqlite3_prepare_v2(_db, "SELECT id, usuario, dia, mes, anio, hora, minuto, segundo, tipo_cuac, mensaje, numero_predefinido FROM cuacs;", -1, &stmt_select, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al preparar SELECT: " << sqlite3_errmsg(_db) << std::endl;
        return cuacs;
    }

    // Iteramos sobre cada fila del resultado
    while (sqlite3_step(stmt_select) == SQLITE_ROW) {

        // Extraemos los campos de cada columna
        int id = sqlite3_column_int(stmt_select, 0);
        
        // sqlite3_column_text devuelve un puntero a unsigned char, lo convertimos a string
        std::string usuario = reinterpret_cast<const char*>(sqlite3_column_text(stmt_select, 1));

        int dia     = sqlite3_column_int(stmt_select, 2);
        int mes     = sqlite3_column_int(stmt_select, 3);
        int anio    = sqlite3_column_int(stmt_select, 4);
        int hora    = sqlite3_column_int(stmt_select, 5);
        int minuto  = sqlite3_column_int(stmt_select, 6);
        int segundo = sqlite3_column_int(stmt_select, 7);

        std::string tipo_cuac = reinterpret_cast<const char*>(sqlite3_column_text(stmt_select, 8));

        // El campo mensaje puede ser NULL en la BBDD, lo manejamos con seguridad
        const char* msg_raw = reinterpret_cast<const char*>(sqlite3_column_text(stmt_select, 9));
        std::string mensaje = msg_raw ? msg_raw : "";

        int numero_predefinido = sqlite3_column_int(stmt_select, 10);

        // Reconstruimos el objeto Fecha a partir de los 6 campos separados
        Fecha f(dia, mes, anio, hora, minuto, segundo);

        // Creamos el Cuac con el constructor de ID explícito (NO incrementa el contador)
        Cuac c(id, usuario, f, tipo_cuac, mensaje, numero_predefinido);
        cuacs.push_back(c);
    }

    sqlite3_finalize(stmt_select);

    return cuacs;
}

/**
 * @brief Inserta un solo cuac en la base de datos SQLite.
 * Usa el prepared statement precompilado para máximo rendimiento O(1).
 * @param cuac Referencia constante al objeto Cuac a persistir.
 */
void Persistencia::insertar(const Cuac& cuac) {

    if (!_db || !_stmt_insertar) return;

    const Fecha& f = cuac.get_fecha();

    // Vinculamos los 11 campos del cuac a los placeholders del prepared statement
    sqlite3_bind_int(_stmt_insertar, 1, cuac.get_id());
    sqlite3_bind_text(_stmt_insertar, 2, cuac.get_usuario().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(_stmt_insertar, 3, f.get_dia());
    sqlite3_bind_int(_stmt_insertar, 4, f.get_mes());
    sqlite3_bind_int(_stmt_insertar, 5, f.get_anio());
    sqlite3_bind_int(_stmt_insertar, 6, f.get_hora());
    sqlite3_bind_int(_stmt_insertar, 7, f.get_minuto());
    sqlite3_bind_int(_stmt_insertar, 8, f.get_segundo());
    sqlite3_bind_text(_stmt_insertar, 9, cuac.get_tipo_cuac().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(_stmt_insertar, 10, cuac.get_texto().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(_stmt_insertar, 11, cuac.get_numero_predefinido());

    // Ejecutamos el INSERT
    int rc = sqlite3_step(_stmt_insertar);

    if (rc != SQLITE_DONE) {
        std::cerr << "[!] Error al insertar cuac #" << cuac.get_id() << ": " << sqlite3_errmsg(_db) << std::endl;
    }

    // Reseteamos el statement para reutilizarlo en la siguiente inserción
    sqlite3_reset(_stmt_insertar);
    sqlite3_clear_bindings(_stmt_insertar);
}

/**
 * @brief Elimina un cuac de la base de datos SQLite por su ID.
 * Usa el prepared statement precompilado para máximo rendimiento O(1).
 * @param id_cuac Identificador del cuac a eliminar.
 */
void Persistencia::eliminar(int id_cuac) {

    if (!_db || !_stmt_eliminar) return;

    // Vinculamos el ID al placeholder del prepared statement
    sqlite3_bind_int(_stmt_eliminar, 1, id_cuac);

    // Ejecutamos el DELETE
    int rc = sqlite3_step(_stmt_eliminar);

    if (rc != SQLITE_DONE) {
        std::cerr << "[!] Error al eliminar cuac #" << id_cuac << ": " << sqlite3_errmsg(_db) << std::endl;
    }

    // Reseteamos el statement para reutilizarlo en la siguiente eliminación
    sqlite3_reset(_stmt_eliminar);
    sqlite3_clear_bindings(_stmt_eliminar);
}

/**
 * @brief Ejecuta PRAGMA integrity_check sobre la base de datos.
 * Verifica la estructura interna de la BBDD y reporta el resultado.
 * @return true si la BBDD está íntegra, false si se detectaron problemas.
 */
bool Persistencia::verificarIntegridad() {

    if (!_db) {
        std::cerr << "[!] No hay conexion activa a la base de datos." << std::endl;
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(_db, "PRAGMA integrity_check;", -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al ejecutar verificacion: " << sqlite3_errmsg(_db) << std::endl;
        return false;
    }

    bool integra = true;

    std::cout << "\n=== Verificacion de Integridad SQLite ===" << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* resultado = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

        if (resultado) {
            std::cout << "  " << resultado << std::endl;

            // Si el resultado no es "ok", hay un problema
            if (std::string(resultado) != "ok") {
                integra = false;
            }
        }
    }

    sqlite3_finalize(stmt);

    if (integra) {
        std::cout << "[i] Base de datos integra. Sin problemas detectados." << std::endl;
    } else {
        std::cerr << "[!] Se detectaron problemas en la base de datos." << std::endl;
    }

    std::cout << "=========================================\n" << std::endl;

    return integra;
}

// === GRAFO SOCIAL (Seguidores) ===

/**
 * @brief Registra una relación de seguimiento en la BBDD.
 * Usa INSERT OR IGNORE: si la relación ya existe, no hace nada (idempotente).
 * @param seguidor Nombre del usuario que sigue.
 * @param seguido Nombre del usuario seguido.
 */
void Persistencia::seguir(const std::string& seguidor, const std::string& seguido) {

    if (!_db || !_stmt_seguir) return;

    // Vinculamos los parámetros: seguidor y seguido
    sqlite3_bind_text(_stmt_seguir, 1, seguidor.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(_stmt_seguir, 2, seguido.c_str(), -1, SQLITE_TRANSIENT);

    // Ejecutamos el INSERT OR IGNORE
    int rc = sqlite3_step(_stmt_seguir);

    if (rc != SQLITE_DONE) {
        std::cerr << "[!] Error al registrar seguimiento: " << sqlite3_errmsg(_db) << std::endl;
    }

    // Reseteamos para reutilizar
    sqlite3_reset(_stmt_seguir);
    sqlite3_clear_bindings(_stmt_seguir);
}

/**
 * @brief Elimina una relación de seguimiento de la BBDD.
 * @param seguidor Nombre del usuario que deja de seguir.
 * @param seguido Nombre del usuario al que se deja de seguir.
 */
void Persistencia::dejarDeSeguir(const std::string& seguidor, const std::string& seguido) {

    if (!_db || !_stmt_dejar) return;

    // Vinculamos los parámetros
    sqlite3_bind_text(_stmt_dejar, 1, seguidor.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(_stmt_dejar, 2, seguido.c_str(), -1, SQLITE_TRANSIENT);

    // Ejecutamos el DELETE
    int rc = sqlite3_step(_stmt_dejar);

    if (rc != SQLITE_DONE) {
        std::cerr << "[!] Error al eliminar seguimiento: " << sqlite3_errmsg(_db) << std::endl;
    }

    // Reseteamos para reutilizar
    sqlite3_reset(_stmt_dejar);
    sqlite3_clear_bindings(_stmt_dejar);
}

/**
 * @brief Carga la lista de usuarios que sigue un usuario dado.
 * @param usuario Nombre del usuario cuya lista de seguidos queremos.
 * @return Lista de nombres de usuarios seguidos.
 */
std::list<std::string> Persistencia::cargarSeguidos(const std::string& usuario) {

    std::list<std::string> seguidos;

    if (!_db) return seguidos;

    // Preparamos un SELECT ad-hoc (no merece prepared statement permanente, se usa pocas veces)
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(_db, "SELECT seguido FROM seguidores WHERE seguidor = ?;", -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al cargar seguidos: " << sqlite3_errmsg(_db) << std::endl;
        return seguidos;
    }

    sqlite3_bind_text(stmt, 1, usuario.c_str(), -1, SQLITE_TRANSIENT);

    // Iteramos sobre los resultados
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* nombre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (nombre) {
            seguidos.push_back(std::string(nombre));
        }
    }

    sqlite3_finalize(stmt);
    return seguidos;
}

/**
 * @brief Carga la lista de seguidores de un usuario dado.
 * @param usuario Nombre del usuario cuyos seguidores queremos.
 * @return Lista de nombres de seguidores.
 */
std::list<std::string> Persistencia::cargarSeguidores(const std::string& usuario) {

    std::list<std::string> seguidores;

    if (!_db) return seguidores;

    // Usamos el índice idx_seguidores_seguido para esta consulta
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(_db, "SELECT seguidor FROM seguidores WHERE seguido = ?;", -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error al cargar seguidores: " << sqlite3_errmsg(_db) << std::endl;
        return seguidores;
    }

    sqlite3_bind_text(stmt, 1, usuario.c_str(), -1, SQLITE_TRANSIENT);

    // Iteramos sobre los resultados
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* nombre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (nombre) {
            seguidores.push_back(std::string(nombre));
        }
    }

    sqlite3_finalize(stmt);
    return seguidores;
}

// === INICIO TESTS ===

void Persistencia::limpiarTodo() {
    this->ejecutar_comando("DELETE FROM cuacs;");
}

void Persistencia::ejecutar_comando(const std::string& sql) {
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "[!] Error SQL en ejecutar_comando (" << sql << "): " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}

// === FIN TESTS ===


#pragma once
#include <string>
#include <list>
#include "Cuac.h"

struct sqlite3;       // Forward declaration (evita incluir sqlite3.h en cabeceras públicas)
struct sqlite3_stmt;

/**
 * @file Persistencia.h
 * @brief Manejador de persistencia SQLite para Cuacker.
 * Mantiene una conexión abierta durante toda la vida de la aplicación y ofrece operaciones incrementales O(1) por cuac.
 */

class Persistencia {

private:
    sqlite3* _db;                    // Conexión activa a la BBDD

    sqlite3_stmt* _stmt_insertar;    // Prepared statement reutilizable para INSERTs de cuacs
    sqlite3_stmt* _stmt_eliminar;    // Prepared statement reutilizable para DELETEs de cuacs

    sqlite3_stmt* _stmt_seguir;      // Prepared statement para INSERT OR IGNORE en seguidores
    sqlite3_stmt* _stmt_dejar;       // Prepared statement para DELETE de seguidores

    /*
    Los PREPARED STATEMENTS son sentencias SQL que se precompilan
    y se reutilizan, lo que mejora el rendimiento.  
    */

    std::string _ruta_db;            // Ruta al fichero .db

    /** @brief Crea la tabla y los índices si no existen. */
    void crearEsquema();

    /** @brief Borra la tabla y los índices si existen (uso interno). */
    void borrarEsquema();

public:

    /**
     * @brief Constructor. Abre la conexión y prepara los statements.
     * @param ruta_db Ruta al fichero de la base de datos SQLite.
     */
    Persistencia(const std::string& ruta_db);

    /** @brief Destructor. Finaliza los statements y cierra la conexión. */
    ~Persistencia();

    /**
     * @brief Carga todos los cuacs almacenados en la BBDD.
     * @return Lista de objetos Cuac reconstruidos desde SQLite.
     */
    std::list<Cuac> cargar();

    /**
     * @brief Inserta un solo cuac en la BBDD (O(1) amortizado).
     * @param cuac El objeto Cuac a persistir.
     */
    void insertar(const Cuac& cuac);

    /**
     * @brief Elimina un cuac de la BBDD por su ID (O(1)).
     * @param id_cuac Identificador (ID) del cuac a eliminar.
     */
    void eliminar(int id_cuac);

    // === GRAFO SOCIAL (Seguidores) ===

    /**
     * @brief Registra una relación de seguimiento en la BBDD.
     * Usa INSERT OR IGNORE para idempotencia (seguir dos veces no es error).
     * @param seguidor Nombre del usuario que sigue.
     * @param seguido Nombre del usuario seguido.
     */
    void seguir(const std::string& seguidor, const std::string& seguido);

    /**
     * @brief Elimina una relación de seguimiento de la BBDD.
     * @param seguidor Nombre del usuario que deja de seguir.
     * @param seguido Nombre del usuario al que se deja de seguir.
     */
    void dejarDeSeguir(const std::string& seguidor, const std::string& seguido);

    /**
     * @brief Carga la lista de usuarios seguidos por un usuario.
     * @param usuario Nombre del usuario cuya lista de seguidos queremos.
     * @return Lista de nombres de usuarios seguidos.
     */
    std::list<std::string> cargarSeguidos(const std::string& usuario);

    /**
     * @brief Carga la lista de seguidores de un usuario.
     * @param usuario Nombre del usuario cuyos seguidores queremos.
     * @return Lista de nombres de seguidores.
     */
    std::list<std::string> cargarSeguidores(const std::string& usuario);

    /**
     * @brief Verifica la salud de la BBDD.
     * @return true si la BBDD está íntegra, false si se detectaron problemas.
     */
    bool verificarIntegridad();

    // === HERRAMIENTAS DE TEST / USO AVANZADO ===

    /** @brief Borra todos los datos de la base de datos (para tests). */
    void limpiarTodo();

    /** @brief Ejecuta un comando SQL genérico (como BEGIN TRANSACTION). */
    void ejecutar_comando(const std::string& sql);
};

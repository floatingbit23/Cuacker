#pragma once
#include <string>
#include <list>
#include <memory>
#include "Cuac.h"

// Forward declarations para SQLite
struct sqlite3;
struct sqlite3_stmt;

/**
 * @brief Deleters personalizados para que std::unique_ptr sepa cómo cerrar recursos de SQLite automáticamente.
 */
struct SQLiteDeleter { void operator()(sqlite3* db) const; };
struct StmtDeleter   { void operator()(sqlite3_stmt* stmt) const; };

/**
 * @file Persistencia.h
 * @brief Manejador de persistencia SQLite para Cuacker.
 * Mantiene una conexión abierta durante toda la vida de la aplicación y ofrece operaciones incrementales O(1) por cuac.
 */

class Persistencia {

private:
    std::unique_ptr<sqlite3, SQLiteDeleter> _db;           // Conexión segura (auto-cierre)

    std::unique_ptr<sqlite3_stmt, StmtDeleter> _stmt_insertar; // Prepared statements con auto-finalize
    std::unique_ptr<sqlite3_stmt, StmtDeleter> _stmt_eliminar;
    std::unique_ptr<sqlite3_stmt, StmtDeleter> _stmt_seguir;
    std::unique_ptr<sqlite3_stmt, StmtDeleter> _stmt_dejar;

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

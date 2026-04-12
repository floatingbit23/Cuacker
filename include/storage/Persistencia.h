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

    sqlite3_stmt* _stmt_insertar;    // Prepared statement reutilizable para INSERTs
    sqlite3_stmt* _stmt_eliminar;    // Prepared statement reutilizable para DELETEs

    /*
    Los PREPARED STATEMENTS son sentencias SQL que se precompilan
    y se reutilizan, lo que mejora el rendimiento.  
    */

    std::string _ruta_db;            // Ruta al fichero .db

    /** @brief Crea la tabla y los índices si no existen. */
    void crearEsquema();

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

    /**
     * @brief Ejecuta PRAGMA (la sentencia SQL, no el #pragma) integrity_check para verificar la salud de la BBDD.
     * @return true si la BBDD está íntegra, false si se detectaron problemas.
     */
    bool verificarIntegridad();
};

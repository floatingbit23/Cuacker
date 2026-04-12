#pragma once

// Incluimos las librerías necesarias para manejar cadenas de texto y fechas
#include <string>
#include "Fecha.h"

/**
 * @brief Clase Cuac
 * En esta clase representamos la unidad básica de nuestra red social: un "cuac".
 * Contiene la información del usuario, el contenido del mensaje y la fecha de publicación.
 */

class Cuac {

    private:
        // Identificador numérico que usamos para mensajes predefinidos (pcuac)
        int _numero_predefinido;

        // Definimos las cadenas para almacenar el contenido, el autor y el tipo de cuac
        std::string _mensaje;    // Contenido textual (limitado idealmente a 140 caracteres)
        std::string _usuario;    // Nombre del usuario que publica
        std::string _tipo_cuac;  // Puede ser "mcuac" (manual) o "pcuac" (predefinido)

        // Almacenamos la fecha de publicación usando nuestra clase Fecha
        Fecha _fecha;

        // Añadimos un ID único y un contador estático (para gestionar el ID autoincremental)
        int _id;
        static int _contador_id;

    public:

        // == CONSTRUCTORES ==

        // Constructor por defecto para inicializar el ID autoincremental
        Cuac();

        // Constructor parametrizado para performance tests (ignorar)
        Cuac(std::string usuario, std::string mensaje, Fecha fecha);

        // Constructor para reconstruir el cuac desde la base de datos (con ID específico)
        Cuac(int id, std::string usuario, Fecha fecha, std::string tipo_cuac, std::string mensaje, int numero_predefinido);

        // == MÉTODOS ==

        /**
         * @brief Leemos un cuac desde la entrada estándar.
         * Según el tipo de cuac que recibamos, gestionamos la lectura del texto o del código numérico.
         */
        bool read_cuac(std::string cuac_type_read);

        /**
         * @brief Mostramos el cuac por pantalla.
         * Nos encargamos de dar formato a la salida para mostrar el usuario, la fecha y el contenido.
         */
        void write_cuac() const;

        // == GETTERS ==

        // Definimos los getters como const y devolvemos por referencia constante
        // para evitar copias innecesarias y garantizar que nadie modifique nuestros datos
        const Fecha& get_fecha() const;
        const std::string& get_usuario() const;

        // get_texto() devuelve por valor porque en el caso 'pcuac' generamos el texto dinámicamente desde el catálogo de mensajes
        std::string get_texto() const;

        // Getters para exportar los datos originales puros a fichero (Persistencia)
        const std::string& get_tipo_cuac() const;
        int get_numero_predefinido() const;

        // Getter del ID del cuac (necesario para el árbol)
        int get_id() const;

        // Sincroniza el contador de IDs con el valor máximo de la base de datos
        static void inicializar_contador(int valor);
};

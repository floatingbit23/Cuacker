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
        int numero001; 

        // Definimos las cadenas para almacenar el contenido, el autor y el tipo de cuac
        std::string mensaje;    // Contenido textual (limitado idealmente a 140 caracteres)
        std::string usuario;    // Nombre del usuario que publica
        std::string cuac_type;  // Puede ser "mcuac" (manual) o "pcuac" (predefinido)

        // Almacenamos la fecha de publicación usando nuestra clase Fecha
        Fecha date;

    public:
        /**
         * @brief Leemos un cuac desde la entrada estándar.
         * Según el tipo de cuac que recibamos, gestionamos la lectura del texto o del código numérico.
         */
        bool read_cuac (std::string cuac_type_read);

        /**
         * @brief Mostramos el cuac por pantalla.
         * Nos encargamos de dar formato a la salida para mostrar el usuario, la fecha y el contenido.
         */
        void write_cuac (void);

        // Definimos los getters para acceder de forma segura a nuestros atributos privados
        Fecha get_fecha();
        std::string get_usuario();
        std::string get_texto();

};

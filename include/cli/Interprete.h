#pragma once

/**
 * @file Interprete.h
 * @brief Definimos nuestra clase Intérprete para la gestión de la interfaz de comandos.
 * En este módulo nos encargamos de recibir las órdenes del usuario, procesarlas
 * y dar la orden correspondiente a nuestro Diccionario de Cuacs.
 */

#include "DiccionarioCuacs.h"

class Interprete {

    private:
        // Mantenemos una instancia de nuestro núcleo de datos
        DiccionarioCuacs _servicio_datos;

        // Nombre del usuario con sesión activa (vacío = sin login, timeline global)
        std::string _usuario_activo;
    public:
        /**
         * @brief Preparamos nuestro intérprete.
         */
        Interprete();

        /**
         * @brief Arrancamos el bucle principal de ejecución de nuestra red social.
         * En este método nos quedamos a la escucha de comandos hasta que se recibe 'exit'.
         */
        void ejecutarCuacker();
};
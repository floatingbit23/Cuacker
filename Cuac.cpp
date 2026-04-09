#include <iostream>
#include <string>
#include "Mensajes.h"
#include "Cuac.h"

// Inicializamos nuestro contador estático de IDs en la unidad de traducción correspondente.
// Decidimos empezar en 1 para que el primer cuac tenga un ID natural.
int Cuac::_contador_id = 1;

/**
 * @brief Constructor por defecto.
 * Cada vez que creamos un nuevo objeto Cuac, le asignamos el valor actual del contador
 * e incrementamos dicho contador para la siguiente instancia.
 */
Cuac::Cuac() {
    _id = _contador_id++;
}

/**
 * @brief Implementamos la lógica de lectura de un Cuac.
 * En este método, decidimos cómo capturar la información dependiendo de si el usuario
 * nos proporciona un cuac manual o uno predefinido.
 * @param cuac_type_read Cadena que indica el tipo de cuac (mcuac o pcuac).
 * @return true si la lectura fue exitosa, false si se detectó el fin de la entrada o un error.
 */
bool Cuac::read_cuac(std::string cuac_type_read){

    // Guardamos el tipo de cuac que estamos procesando
    _tipo_cuac = cuac_type_read;

    // Intentamos leer el nombre del usuario; si no hay más entrada, detenemos el proceso
    if(!(std::cin >> _usuario)){
	    return false; // Si no se puede leer el usuario, salimos del bucle
	}

    // Procedemos a leer la fecha asociada al cuac usando nuestro método de clase Fecha
    _fecha.leer_fecha();

    // Dependiendo del tipo de cuac, gestionamos el cuerpo del mensaje de forma distinta
    if (_tipo_cuac == "mcuac"){

        // Limpiamos el buffer para evitar problemas con saltos de línea y leemos toda la cadena
        std::cin.ignore();
        std::getline(std::cin, _mensaje);

    } else if (_tipo_cuac == "pcuac"){

        // Si es un cuac predefinido, simplemente leemos el índice numérico
        std::cin >> _numero_predefinido;
        std::cin.ignore();

    }

    return true;
}

/**
 * @brief Mostramos la información de nuestro Cuac con un formato legible.
 * Ahora usamos el ID autogenerado para identificar cada publicación de forma única.
 */
void Cuac::write_cuac() const {

    // Mostramos el ID del cuac al principio para facilitar el seguimiento
    std::cout << "[ID:" << _id << "] " << _usuario << " " << _fecha << "\n"; // Ej. [ID:1] John 01/01/2022

    // Mostramos el mensaje con una pequeña sangría para mejorar la visualización
    if (_tipo_cuac == "mcuac"){

        std::cout << "   " << _mensaje;

    } else if (_tipo_cuac == "pcuac"){

        std::cout << "   ";
        // Recuperamos el mensaje predefinido del catálogo
        imprime_mensaje(_numero_predefinido);

    }

}

/**
 * @brief Recuperamos nuestra fecha de publicación por referencia constante.
 */
const Fecha& Cuac::get_fecha() const {
    return _fecha;
}

/**
 * @brief Recuperamos el nombre del usuario autor por referencia constante.
 */
const std::string& Cuac::get_usuario() const {
    return _usuario;
}

/**
 * @brief Recuperamos el cuerpo del mensaje.
 * Devolvemos por valor porque en el caso pcuac necesitamos generar una cadena nueva a partir de nuestro catálogo de mensajes.
 */
std::string Cuac::get_texto() const {
    // Determinamos qué texto devolver basándonos en si almacenamos una cadena o un índice
    if (_tipo_cuac == "mcuac"){
        return _mensaje;
    } else if (_tipo_cuac == "pcuac"){
        // Recuperamos el mensaje predefinido a partir de nuestro catálogo numérico
        return obtiene_mensaje(_numero_predefinido);
    }

    return "";
}

/**
 * @brief Recuperamos el ID único de este Cuac.
 */
int Cuac::get_id() const {
    return _id;
}

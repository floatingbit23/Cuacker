#include <iostream>
#include <string>
#include "Mensajes.h"
#include "Cuac.h"

/**
 * @brief Implementamos la lógica de lectura de un Cuac.
 * En este método, decidimos cómo capturar la información dependiendo de si el usuario
 * nos proporciona un cuac manual o uno predefinido.
 * @return true si la lectura fue exitosa, false si se detectó el fin de la entrada o un error.
 */
bool Cuac::read_cuac (std::string cuac_type_read){

    // Guardamos el tipo de cuac que estamos procesando
    cuac_type = cuac_type_read;

    // Intentamos leer el nombre del usuario; si no hay más entrada, detenemos el proceso
    // El nombre de usuario no podrá contener espacios!
    if(!(std::cin >> usuario)){ 
		return false; // Si no se puede leer el usuario, salimos del bucle
	}

    // Procedemos a leer la fecha asociada al cuac usando nuestro método de clase Fecha
    date.leer_fecha();

    // Dependiendo del tipo de cuac, gestionamos el cuerpo del mensaje de forma distinta
    if (cuac_type == "mcuac"){

        // Limpiamos el buffer para evitar problemas con saltos de línea y leemos toda la cadena
        std::cin.ignore();
        std::getline(std::cin, mensaje);

    } else if (cuac_type == "pcuac"){

        // Si es un cuac predefinido, simplemente leemos el índice numérico
        std::cin >> numero001;
        std::cin.ignore();

    }

    return true;
}

/**
 * @brief Mostramos la información de nuestro Cuac con un formato legible.
 * Imprimimos el autor seguido de la fecha y, en una nueva línea, el contenido del mensaje.
 */
void Cuac::write_cuac (void){

    std::cout << usuario << " ";
    date.escribir_fecha();
    std::cout << "\n";

    // Mostramos el mensaje con una pequeña sangría para mejorar la visualización
    if (cuac_type == "mcuac"){

        std::cout << "   " << mensaje; // Ejemplo: "   Hola, este es mi primer cuac"

    } else if (cuac_type == "pcuac"){ 

        std::cout << "   ";
        // Si es predefinido, llamamos a nuestra función auxiliar para imprimir el texto correspondiente
        
        imprime_mensaje(numero001); // Ejemplo: "   ¡Qué día tan maravilloso para programar!"

    }

}

/**
 * @brief Recuperamos nuestra fecha de publicación.
 * @return Un objeto Fecha con el momento exacto en que se creó el cuac.
 */
Fecha Cuac::get_fecha(){
    return date;
}

/**
 * @brief Recuperamos el nombre del usuario autor.
 * @return Una cadena de texto con el nombre de usuario del autor del cuac.
 */
std::string Cuac::get_usuario(){
    return usuario;
}

/**
 * @brief Recuperamos el cuerpo del mensaje.
 * @return El texto del mensaje (manual o el recuperado del catálogo si es predefinido).
 */
std::string Cuac::get_texto(){
    // Determinamos qué texto devolver basándonos en si almacenamos una cadena o un índice
    if (cuac_type == "mcuac"){
        return mensaje;
    } else if (cuac_type == "pcuac"){
        // Recuperamos el mensaje predefinido a partir de nuestro catálogo numérico
        return obtiene_mensaje(numero001);
    }

    return ""; 
}


#include <iostream>
#include "Fecha.h"

/**
 * @brief Implementamos la lectura de la fecha.
 * En nuestro diseño, decidimos manejar la entrada saltándonos los caracteres
 * delimitadores (como '/' o ':') mediante una variable auxiliar.
 */

void Fecha::leer_fecha(){

    char ce; // Variable que usamos para "consumir" los caracteres especiales que no necesitamos
    std::cin >> dia >> ce >> mes >> ce >> ano >> hora >> ce >> minuto >> ce >> segundo;

}

/**
 * @brief Mostramos la fecha formateada.
 * Nos aseguramos de mantener un formato de dos dígitos para las horas, minutos
 * y segundos, añadiendo ceros a la izquierda cuando sea necesario (ej: 09:05:02).
 */

void Fecha::escribir_fecha(){

    std::cout << dia << "/" << mes << "/" << ano << " "; // ej. 09/05/2026

    // Gestionamos el formato de la hora para que siempre tenga dos dígitos
    if (hora < 10) {
        std::cout << 0 << hora << ":";
    } else {
        std::cout << hora << ":";
    }

    if (minuto < 10) {
        std::cout << 0 << minuto << ":";
    } else {
        std::cout << minuto << ":";
    }

    if (segundo < 10) {
        std::cout << 0 << segundo;
    } else {
        std::cout << segundo;
    }

}

/**
 * @brief Comparamos si nuestra fecha es anterior a otra.
 * Realizamos una comprobación jerárquica: empezamos por el año y vamos bajando 
 * hasta los segundos para determinar cuál sucedió primero.
 * @return true si nuestra fecha es cronológicamente anterior a 'otra', false en caso contrario.
 */
bool Fecha::es_menor(Fecha &otra) {

    // Primero comparamos los años
    if (ano < otra.ano) {
        return true;
    } else if (ano > otra.ano) {
        return false; 
    }

    // Si los años coinciden, pasamos a comparar los meses
    if (mes < otra.mes) {
        return true;
    } else if (mes > otra.mes) {
        return false;
    }

    // Continuamos con los días si el mes es el mismo
    if (dia < otra.dia) {
            return true;
    } else if (dia > otra.dia) {
            return false;
    }

    // Seguimos el mismo proceso para las horas...
    if (hora < otra.hora) {
            return true;
    } else if (hora > otra.hora) {
        return false;
    }

    // ... los minutos ...
    if (minuto < otra.minuto) {
            return true;
    } else if (minuto > otra.minuto) {
            return false;
    }

    // ... y finalmente los segundos
    if (segundo < otra.segundo) {
            return true;
    } else {
        return false;
    }
}

/**
 * @brief Verificamos si dos fechas son exactamente iguales.
 * Para nosotros, son iguales si todos y cada uno de sus componentes coinciden.
 * @return true si todos los componentes temporales coinciden, false si hay alguna diferencia.
 */
bool Fecha::es_igual(Fecha &otra) {

    return (ano == otra.ano) && (mes == otra.mes) && (dia == otra.dia) &&
            (hora == otra.hora) && (minuto == otra.minuto) && (segundo == otra.segundo);

}

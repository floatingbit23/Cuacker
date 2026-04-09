#include <iostream>
#include "Fecha.h"

/**
 * @brief Implementamos la lectura de la fecha.
 * En nuestro diseño, decidimos manejar la entrada saltándonos los caracteres
 * delimitadores (como '/' o ':') mediante una variable auxiliar.
 */

void Fecha::leer_fecha(){
    // 'separador' es una variable auxiliar que usamos para "consumir" los caracteres especiales que no necesitamos
    char separador; 
    std::cin >> _dia >> separador >> _mes >> separador >> _anio >> _hora >> separador >> _minuto >> separador >> _segundo;
}

/**
 * @brief Implementamos la salida formateada mediante el operador de flujo.
 * Nos aseguramos de mantener un formato de dos dígitos para las horas, minutos
 * y segundos, añadiendo ceros a la izquierda cuando sea necesario.
 * @param flujo_salida Referencia al flujo de salida (fichero o consola).
 * @param fecha Referencia constante a la fecha que queremos imprimir.
 * @return El flujo de salida para permitir encadenamiento.
 */

 // Sobrecargamos el Operador de Flujo de Salida para poder imprimir fechas directamente
std::ostream& operator<<(std::ostream& flujo_salida, const Fecha& fecha) {

    flujo_salida << fecha._dia << "/" << fecha._mes << "/" << fecha._anio << " ";

    // Gestionamos el formato de la hora para que siempre tenga dos dígitos
    if (fecha._hora < 10) flujo_salida << 0;
    flujo_salida << fecha._hora << ":";

    if (fecha._minuto < 10) flujo_salida << 0;
    flujo_salida << fecha._minuto << ":";

    if (fecha._segundo < 10) flujo_salida << 0;
    flujo_salida << fecha._segundo;

    return flujo_salida; // Devolvemos el flujo para permitir encadenamiento (ej: cout << fecha1 << fecha2;)
}

/**
 * @brief Mantenemos escribir_fecha() por compatibilidad.
 * Internamente delegamos en nuestro operator<< para evitar duplicar lógica.
 */
void Fecha::escribir_fecha() const {
    std::cout << *this;
}

/**
 * @brief Comparamos si nuestra fecha es estrictamente anterior a otra.
 * Realizamos una comprobación jerárquica empezando por el año.
 * @param otra_fecha Referencia a la fecha con la que queremos comparar la nuestra.
 * @return Verdadero si nuestra fecha es anterior, falso en caso contrario.
 */
bool Fecha::operator<(const Fecha& otra_fecha) const {
    
    // Primero comparamos los años
    if (_anio < otra_fecha._anio) return true;
    if (_anio > otra_fecha._anio) return false;

    // Si los años coinciden, pasamos a comparar los meses
    if (_mes < otra_fecha._mes) return true;
    if (_mes > otra_fecha._mes) return false;

    // Continuamos con los días si el mes es el mismo
    if (_dia < otra_fecha._dia) return true;
    if (_dia > otra_fecha._dia) return false;

    // Seguimos el mismo proceso para las horas...
    if (_hora < otra_fecha._hora) return true;
    if (_hora > otra_fecha._hora) return false;

    // ... los minutos ...
    if (_minuto < otra_fecha._minuto) return true;
    if (_minuto > otra_fecha._minuto) return false;

    // ... y finalmente los segundos
    if (_segundo < otra_fecha._segundo) return true;
    return false;
}


/**
 * @brief Verificamos si dos fechas son exactamente iguales.
 * @param otra_fecha Referencia a la fecha con la que queremos comparar la nuestra.
 */
bool Fecha::operator==(const Fecha& otra_fecha) const {
    return (_anio == otra_fecha._anio) && (_mes == otra_fecha._mes) && (_dia == otra_fecha._dia) &&
           (_hora == otra_fecha._hora) && (_minuto == otra_fecha._minuto) && (_segundo == otra_fecha._segundo);
}

// Derivamos el resto de operadores a partir de los dos principales.

bool Fecha::operator>(const Fecha& otra_fecha) const {
    return otra_fecha < *this;
}

bool Fecha::operator<=(const Fecha& otra_fecha) const {
    return !(otra_fecha < *this);
}

bool Fecha::operator>=(const Fecha& otra_fecha) const {
    return !(*this < otra_fecha);
}

bool Fecha::operator!=(const Fecha& otra_fecha) const {
    return !(*this == otra_fecha);
}

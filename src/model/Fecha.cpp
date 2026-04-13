#include "Fecha.h"
#include <iomanip>
#include <iostream>

/**
 * @file Fecha.cpp
 * @brief Implementación de la clase Fecha para la gestión cronológica de Cuacker.
 * 
 * En este archivo implementamos la lógica de comparación, lectura e impresión de
 * fechas, utilizando optimizaciones de paso por valor para mejorar el rendimiento
 * en arquitecturas de hardware modernas.
 */

/**
 * @brief Constructor por defecto.
 * Inicializa la fecha en la época Unix (01/01/1970 00:00:00).
 */
Fecha::Fecha() : _dia(1), _mes(1), _anio(1970), _hora(0), _minuto(0), _segundo(0) {}

/**
 * @brief Constructor parametrizado.
 * Permite inicializar todos los campos de la fecha manualmente.
 * @param d Día.
 * @param m Mes.
 * @param a Año.
 * @param h Hora.
 * @param min Minutos.
 * @param seg Segundos.
 */
Fecha::Fecha(int d, int m, int a, int h, int min, int seg) 
    : _dia(d), _mes(m), _anio(a), _hora(h), _minuto(min), _segundo(seg) {}

/**
 * @brief Captura la fecha desde el flujo de entrada estándar.
 * Espera un formato numérico con separadores (ej: 01/01/2026 12:00:00).
 * @return true si la lectura fue exitosa, false si ocurrió un error de formato.
 */
bool Fecha::leer_fecha() {
    char separador; 
    if (!(std::cin >> _dia >> separador >> _mes >> separador >> _anio >> _hora >> separador >> _minuto >> separador >> _segundo)) {
        return false;
    }
    return true;
}

/**
 * @brief Escribe la fecha en la salida estándar con formato DD/MM/AAAA HH:MM:SS.
 */
void Fecha::escribir_fecha() const {
    std::cout << *this << std::endl;
}

/**
 * @brief Sobrecarga del operador de menor que (<).
 * Realiza una comparación jerárquica (Año > Mes > Día > Hora > Minuto > Segundo).
 * @param otra Objeto Fecha a comparar (pasado por valor para optimización).
 * @return true si la fecha actual es cronológicamente anterior a 'otra'.
 */
bool Fecha::operator<(Fecha otra) const {
    if (_anio != otra._anio) return _anio < otra._anio;
    if (_mes != otra._mes) return _mes < otra._mes;
    if (_dia != otra._dia) return _dia < otra._dia;
    if (_hora != otra._hora) return _hora < otra._hora;
    if (_minuto != otra._minuto) return _minuto < otra._minuto;
    return _segundo < otra._segundo;
}

/**
 * @brief Sobrecarga del operador de igualdad (==).
 * @param otra Objeto Fecha a comparar (pasado por valor).
 * @return true si todos los campos temporales coinciden exactamente.
 */
bool Fecha::operator==(Fecha otra) const {
    return (_anio == otra._anio && _mes == otra._mes && _dia == otra._dia &&
            _hora == otra._hora && _minuto == otra._minuto && _segundo == otra._segundo);
}

/**
 * @brief Sobrecarga del operador de mayor que (>).
 * @param otra Objeto Fecha a comparar.
 * @return true si la fecha actual es posterior a 'otra'.
 */
bool Fecha::operator>(Fecha otra) const {
    return otra < *this;
}

/**
 * @brief Sobrecarga del operador de menor o igual que (<=).
 * @param otra Objeto Fecha a comparar.
 * @return true si la fecha actual es anterior o igual a 'otra'.
 */
bool Fecha::operator<=(Fecha otra) const {
    return !(*this > otra);
}

/**
 * @brief Sobrecarga del operador de mayor o igual que (>=).
 * @param otra Objeto Fecha a comparar.
 * @return true si la fecha actual es posterior o igual a 'otra'.
 */
bool Fecha::operator>=(Fecha otra) const {
    return !(*this < otra);
}

/**
 * @brief Sobrecarga del operador de desigualdad (!=).
 * @param otra Objeto Fecha a comparar.
 * @return true si las fechas son distintas.
 */
bool Fecha::operator!=(Fecha otra) const {
    return !(*this == otra);
}

/**
 * @brief Inserción en flujo de salida (impresión formateada).
 * Utiliza manipuladores de flujo para garantizar que los números tengan siempre 2 dígitos.
 * @param os Flujo de salida.
 * @param f Objeto Fecha a imprimir.
 * @return Referencia al flujo modificado.
 */
std::ostream& operator<<(std::ostream& os, const Fecha& f) {
    os << std::setfill('0') << std::setw(2) << f._dia << "/"
       << std::setw(2) << f._mes << "/"
       << f._anio << " "
       << std::setw(2) << f._hora << ":"
       << std::setw(2) << f._minuto << ":"
       << std::setw(2) << f._segundo;
    return os;
}

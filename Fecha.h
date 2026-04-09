#pragma once

#include <iostream>

/**
 * @file Fecha.h
 * @brief Definimos nuestra clase para la gestión de fechas y tiempo.
 * En este módulo nos encargamos de encapsular toda la lógica relativa al manejo
 * cronológico de nuestras publicaciones.
 */

class Fecha {

	private:
		// Almacenamos cada componente de la fecha y hora de forma individual
		// Usamos el prefijo '_' para distinguir claramente los atributos de la clase
		int _dia, _mes, _anio, _hora, _minuto, _segundo;

	public:
		/**
		 * @brief Capturamos la fecha desde el flujo de entrada.
		 * Definimos un formato específico para que nuestra lectura sea consistente.
		 */
		void leer_fecha();

		/**
		 * @brief Mostramos la fecha con un formato amigable para el usuario.
		 * Mantenemos este método por compatibilidad; internamente delegamos en operator<<.
		 */
		void escribir_fecha() const;

		/**
		 * @brief Realizamos comparaciones cronológicas mediante sobrecarga de operadores.
		 * Pasamos las fechas como parámetros constantes para evitar copias innecesarias, ya que no vamos a modificarlas.
		 */
		bool operator<(const Fecha& otra_fecha) const;
		bool operator==(const Fecha& otra_fecha) const;
		bool operator>(const Fecha& otra_fecha) const;
		bool operator<=(const Fecha& otra_fecha) const;
		bool operator>=(const Fecha& otra_fecha) const;
		bool operator!=(const Fecha& otra_fecha) const;

		/**
		 * @brief Función amiga de la clase Fecha (porque la función operator<< debe poder acceder a los atributos privados de dicha clase) 
		 * Esta función permite la impresión directa de fechas mediante flujos.
		 * Con esto podemos escribir: cout << miFecha;
		 */
		friend std::ostream& operator<<(std::ostream& flujo_salida, const Fecha& fecha);
};

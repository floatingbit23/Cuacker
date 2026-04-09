#pragma once

/**
 * @file Fecha.h
 * @brief Definimos nuestra clase para la gestión de fechas y tiempo.
 * En este módulo nos encargamos de encapsular toda la lógica relativa al manejo
 * cronológico de nuestras publicaciones.
 */

class Fecha {

	private:
		// Almacenamos cada componente de la fecha y hora de forma individual
		int dia, mes, ano, hora, minuto, segundo;

	public:
		/**
		 * @brief Capturamos la fecha desde el flujo de entrada.
		 * Definimos un formato específico para que nuestra lectura sea consistente.
		 */
		void leer_fecha();

		/**
		 * @brief Mostramos la fecha con un formato amigable para el usuario.
		 */
		void escribir_fecha();

		/**
		 * @brief Realizamos comparaciones cronológicas.
		 * Estos métodos nos permiten decidir qué publicación es más antigua o si coinciden.
		 */
		bool es_menor(Fecha &otra);
		bool es_igual(Fecha &otra);
};



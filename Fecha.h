// Módulo del 003.cpp
#ifndef FECHA_H
#define FECHA_H

// Clase Fecha
class Fecha {

	private:
		int dia, mes, ano, hora, minuto, segundo;

	public:

		void leer_fecha();
		void escribir_fecha();
		bool es_menor(Fecha &otra);
		bool es_igual(Fecha &otra);
};

#endif



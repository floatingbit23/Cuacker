#ifndef CUAC_H
#define CUAC_H

#include <string>

using namespace std;

#include "Fecha.h" // para acceder al tipo Fecha

// Clase
class Cuac {

    private:

        int numero001; // numero del cuac (sustituído por texto predefinido en el ejercicio 1)

        string mensaje; // mensaje del cuac (<= 140 char)
        string usuario; // usuario del cuac
        string cuac_type; // mcuac o pcuac

        Fecha date; // fecha del cuac (objeto de clase Fecha)

    public:

        // Constructor por defecto

        // Declaración de métodos
        bool read_cuac (string cuac_type_read);
        void write_cuac (void);

        // Getters
        Fecha get_fecha();
        string get_usuario();
        string get_texto();

};

bool read_cuac(std::string cuac_type_read);

void write_cuac(void);

#endif

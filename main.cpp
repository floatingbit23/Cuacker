/**
 * @file main.cpp
 * @brief Punto de entrada de nuestra aplicación Cuacker.
 * En este archivo nos encargamos de instanciar nuestro motor principal
 * y arrancar el flujo de ejecución de nuestra red social.
 */

#include "Interprete.h"

int main() {

  /**
   * @brief Creamos nuestro objeto Intérprete.
   * Este objeto centralizará toda la interacción con el usuario.
   */

  Interprete cuacker;

  /**
   * @brief Iniciamos la ejecución de nuestro programa.
   * Aquí le cedemos el control al intérprete de comandos.
   */

  cuacker.ejecutarCuacker(); // Arrancamos el REPL (Read-Eval-Print Loop)

  return 0; // Finalizamos nuestra aplicación correctamente
}

#pragma once

/**
 * @file Mensajes.h
 * @brief Declaramos las funciones para gestionar nuestro catálogo de mensajes predefinidos.
 * En nuestro sistema, utilizamos colecciones de textos ya escritos para facilitar la 
 * comunicación rápida de nuestros usuarios.
 */
#include <string>

/**
 * @brief Imprimimos por pantalla el mensaje asociado a un índice determinado.
 */
void imprime_mensaje(int number);

/**
 * @brief Recuperamos la cadena de texto de nuestro catálogo a partir de su número identificador.
 */
std::string obtiene_mensaje(int number);


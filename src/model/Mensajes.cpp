#include <iostream>
#include <string>
#include "Mensajes.h"

/**
 * @brief Definimos nuestro almacén de mensajes predefinidos.
 * Hemos seleccionado una serie de frases comunes que nuestros usuarios pueden publicar 
 * simplemente referenciando su número de orden, lo que nos permite ahorrar espacio.
 * @return El mensaje correspondiente al número solicitado o una cadena vacía si no existe.
 */
std::string obtiene_mensaje(int number){

    // Definimos nuestra lista estática de mensajes disponibles en el sistema
	const std::string mensajes[30] = {

		    "Afirmativo.", // mensajes[0]
        "Negativo.", // mensajes[1]
        "Estoy de viaje en el extranjero.", // ...
        "Muchas gracias a todos mis seguidores por vuestro apoyo.",
        "Enhorabuena, campeones!",
        "Ver las novedades en mi pagina web.",
        "Estad atentos a la gran exclusiva del siglo.",
        "La inteligencia me persigue pero yo soy mas rapido.",
        "Si no puedes convencerlos, confundelos.",
        "La politica es el arte de crear problemas.",
        "Donde estan las llaves, matarile, rile, rile...",
        "Si no te gustan mis principios, puedo cambiarlos por otros.",
        "Un dia lei que fumar era malo y deje de fumar.",
        "Yo si se lo que es trabajar duro, de verdad, porque lo he visto por ahi.",
        "Hay que trabajar ocho horas y dormir ocho horas, pero no las mismas.",
        "Mi vida no es tan glamurosa como mi pagina web aparenta.",
        "Todo tiempo pasado fue anterior.",
        "El azucar no engorda... engorda el que se la toma.",
        "Solo los genios somos modestos.",
        "Nadie sabe escribir tambien como yo.",
        "Si le molesta el mas alla, pongase mas aca.",
        "Me gustaria ser valiente. Mi dentista asegura que no lo soy.",
        "Si el dinero pudiera hablar, me diria adios.",
        "Hoy me ha pasado una cosa tan increible que es mentira.",
        "Si no tienes nada que hacer, por favor no lo hagas en clase.",
        "Que nadie se vanaglorie de su justa y digna raza, que pudo ser un melon y salio una calabaza.",
        "Me despido hasta la proxima. Buen viaje!",
        "Cualquiera se puede equivocar, inclusivo yo.",
        "Estoy en Egipto. Nunca habia visto las piramides tan solas.",
        "El que quiera saber mas, que se vaya a Salamanca." // mensajes[29]
    };

  // Verificamos que el número solicitado esté dentro de nuestro rango válido (1-30)
  if(number >=1 && number <= 30){
		return mensajes[number-1]; // Restamos 1 para ajustar al índice de nuestro array
  }

  // Si el código no existe en nuestra lista, devolvemos una cadena vacía
  return "";

}

/**
 * @brief Enviamos directamente al flujo de salida el mensaje solicitado.
 */
void imprime_mensaje(int number){
  std::cout << obtiene_mensaje(number);
}

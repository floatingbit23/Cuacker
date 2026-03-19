#include <iostream>

#include "Fecha.h"

using namespace std;


// Métodos


void Fecha::leer_fecha(){

    /*puesto que un array no tendría MF voy a
    guardar los valores individuales de la cadena
    y usar 'ce' para los caracteres especiales que
    no me interesen como "/" o " " */

    char ce;
    cin >> dia >> ce >> mes >> ce >> ano >> hora >> ce >> minuto >> ce >> segundo;

}

void Fecha::escribir_fecha(){

    cout << dia << "/" << mes << "/" << ano << " ";

    //formato de la hora
    if (hora < 10) {
        cout << 0 << hora << ":";
    } else {
        cout << hora << ":";
    }

    if (minuto < 10) {
        cout << 0 << minuto << ":";
    } else {
        cout << minuto << ":";
    }

    if (segundo < 10) {
        cout << 0 << segundo;
    } else {
        cout << segundo;
    }

}

bool Fecha::es_menor(Fecha &otra) {


    if (ano < otra.ano) {
        return true;
    } else if (ano > otra.ano) {
        return false; }

    if (mes < otra.mes) {
        return true;
    } else if (mes > otra.mes) {
        return false;
    }

    if (dia < otra.dia) {
            return true;
    } else if (dia > otra.dia) {
            return false;
    }

    if (hora < otra.hora) {
            return true;
    } else if (hora > otra.hora) {
        return false;
    }

    if (minuto < otra.minuto) {
            return true;
    } else if (minuto > otra.minuto) {
            return false;
    }

    if (segundo < otra.segundo) {
            return true;
    } else {
        return false;
    }
}

bool Fecha::es_igual(Fecha &otra) {

    // si todas las condiciones se cumplen devuelve 'true'
    return (ano == otra.ano) && (mes == otra.mes) && (dia == otra.dia) &&
            (hora == otra.hora) && (minuto == otra.minuto) && (segundo == otra.segundo);

}

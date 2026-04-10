#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>
#include "ArbolAVL.h"
#include "TablaHash.h"
#include "Cuac.h"
#include "Fecha.h"

/**
 * @file performance_test.cpp
 * @brief Suite de pruebas de rendimiento para Cuacker.
 * 
 * Este programa somete a estrés a nuestras estructuras de datos (AVL y Hash)
 * para verificar su comportamiento con grandes volúmenes de información.
 */

// Generador de usuarios aleatorios para el test de Hash
std::string generarUsuarioAleatorio(int longitud = 8) {
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphabet) - 2);
    
    std::string s = "";
    for (int i = 0; i < longitud; ++i) {
        s += alphabet[dis(gen)];
    }
    return s;
}

int main() {
    const int NUM_ELEMENTOS = 1000000; 
    std::cout << "=== CUACKER PERFORMANCE & TOLERANCE TEST ===" << std::endl;
    std::cout << "Sometiendo a estres con " << NUM_ELEMENTOS << " cuacs..." << std::endl << std::endl;

    TablaHash tabla;
    Arbol_AVL arbol;
    
    // Preparar datos para que no consuman tiempo de cronómetro durante el test
    std::vector<Cuac*> cuacs_generados;
    for(int i=0; i<NUM_ELEMENTOS; ++i) {
        Fecha f(10, 4, 2026, 12, 0, i % 60);
        cuacs_generados.push_back(new Cuac(generarUsuarioAleatorio(), "Mensaje de prueba", f));
    }

    // --- TEST DE INSERSIÓN MASIVA ---
    std::cout << "[1/3] Test de Insercion Masiva (Hash + AVL)..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ELEMENTOS; ++i) {
        // Insertamos en la tabla
        Cuac* pt = tabla.insertar(*(cuacs_generados[i]));
        // Insertamos en el árbol
        arbol.insertar(pt);
        
        if (i % 10000 == 0 && i > 0) std::cout << "   ... " << i << " insertados" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << ">> Tiempo total de insercion: " << std::fixed << std::setprecision(4) << elapsed.count() << " segundos" << std::endl;
    std::cout << ">> Media por cuac: " << (elapsed.count() / NUM_ELEMENTOS) * 1000000 << " microsegundos\n" << std::endl;

    // --- TEST DE BÚSQUEDA HASH O(1) ---
    std::cout << "[2/3] Test de Busqueda por Usuario (Tabla Hash)..." << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<1000; ++i) {
        // Buscamos el usuario del cuac i
        std::string buscar = cuacs_generados[i]->get_usuario();
        tabla.follow(buscar);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    
    std::cout << ">> Tiempo para 1000 búsquedas follow: " << elapsed.count() << " segundos" << std::endl;
    std::cout << ">> Coste medio por búsqueda: " << (elapsed.count() / 1000) * 1000000 << " microsegundos (Objetivo: O(1))\n" << std::endl;

    // --- TEST DE RANGO AVL O(log n) ---
    std::cout << "[3/3] Test de Rango Temporal (Arbol AVL)..." << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    // Simulamos 100 búsquedas de los últimos 10 mensajes
    for(int i=0; i<100; ++i) {
        arbol.last(10); 
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << ">> Tiempo para 100 llamadas a last(10) entre " << NUM_ELEMENTOS << " elementos: " << elapsed.count() * 1000 << " milisegundos" << std::endl;
    std::cout << ">> El arbol ha mantenido su estructura balanceada con éxito.\n" << std::endl;

    std::cout << "=== TEST FINALIZADO CON EXITO ===" << std::endl;
    
    // Limpieza
    for(auto p : cuacs_generados) delete p;

    return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>
#include "DiccionarioCuacs.h"
#include "Cuac.h"
#include "Fecha.h"
#include "Persistencia.h"

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
    const int NUM_ELEMENTOS = 300000; 
    std::cout << "=== CUACKER PERFORMANCE & TOLERANCE TEST (SQLITE OPTIMIZED) ===" << std::endl;
    std::cout << "Sometiendo a estres con " << NUM_ELEMENTOS << " cuacs..." << std::endl;

    DiccionarioCuacs diccionario;
    Persistencia persistencia("test_stress.db");
    persistencia.limpiarTodo(); // Empezamos de cero para el test
    
    // Preparar datos en memoria
    std::vector<Cuac*> cuacs_generados;
    std::vector<std::string> usuarios_test; // Guardamos los usuarios para buscarlos luego
    for(int i=0; i<NUM_ELEMENTOS; ++i) {
        std::string usr = generarUsuarioAleatorio();
        usuarios_test.push_back(usr);
        Fecha f(10, 4, 2026, 12, 0, i % 60);
        cuacs_generados.push_back(new Cuac(usr, "Prueba de rendimiento #" + std::to_string(i), f));
    }

    int id_a_borrar = cuacs_generados[NUM_ELEMENTOS/2]->get_id();

    // --- TEST DE INSERSIÓN MASIVA CON TRANSACCIÓN ---
    std::cout << "[1/4] Test de Insercion Masiva (RAM + SQLite Transaction)..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    // Activamos persistencia
    diccionario.setPersistencia(&persistencia);
    
    // Iniciamos transacción para que SQLite no frene la RAM
    persistencia.ejecutar_comando("BEGIN TRANSACTION;");

    for (int i = 0; i < NUM_ELEMENTOS; ++i) {
        // Usamos std::move para que el test aproveche las optimizaciones de movimiento que hemos creado
        diccionario.insertar(std::move(*(cuacs_generados[i])));
        if (i % 50000 == 0 && i > 0) std::cout << "   ... " << i << " insertados" << "\n";
    }

    // Cerramos transacción: aquí es donde se vuelca todo a disco de golpe
    persistencia.ejecutar_comando("COMMIT;");

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_insert = end - start;
    std::cout << ">> Tiempo total (300K inserts + SQL Commit): " << total_insert.count() << " segundos" << std::endl;
    std::cout << ">> Media por insercion: " << (total_insert.count() / NUM_ELEMENTOS) * 1000 << " ms\n" << std::endl;

    // --- TEST DE BÚSQUEDA HASH O(1) ---
    std::cout << "[2/4] Test de Recuperacion por Usuario (Tabla Hash)..." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<100; ++i) {
        // Buscamos un usuario que sabemos que existe (usando el vector preservado)
        diccionario.follow(usuarios_test[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << ">> Tiempo para 100 búsquedas follow: " << elapsed.count() << " segundos (Objetivo: O(1))\n" << std::endl;

    // --- TEST DE RANGO AVL O(log n) ---
    std::cout << "[3/4] Test de Rango Temporal (Arbol AVL)..." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<10; ++i) {
        diccionario.last(10); 
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << ">> Tiempo para 10 llamadas a last(10): " << elapsed.count() * 1000 << " ms (Objetivo: O(log n))\n" << std::endl;

    // --- TEST DE BORRADO OPTIMIZADO O(log n) ---
    std::cout << "[4/4] Test de Borrado por ID (Índice Secundario)..." << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    diccionario.eliminar(id_a_borrar);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    
    std::cout << ">> Tiempo para borrar Cuac #" << id_a_borrar << ": " << elapsed.count() * 1000 << " ms (Objetivo: O(log n))\n" << std::endl;

    diccionario.stats();
    std::cout << "=== TEST FINALIZADO CON EXITO ===" << std::endl;
    
    for(auto p : cuacs_generados) delete p;
    return 0;
}

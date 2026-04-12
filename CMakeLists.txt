cmake_minimum_required(VERSION 3.10)

# Nombre de tu proyecto y lenguaje
project(Cuacker LANGUAGES C CXX)

# Exigimos el estándar C++11 (o superior para las listas de inicialización y otras mejoras)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadimos los directorios de cabeceras para que los #include funcionen sin modificar la ruta relativa en cada fichero
include_directories(include include/model include/storage include/cli lib)

# Buscamos automáticamente todos los archivos .cpp y .h en la nueva estructura de carpetas
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

# Compilamos SQLite como una librería estática (C puro, sin warnings de C++)
add_library(sqlite3 STATIC lib/sqlite3.c)
target_compile_definitions(sqlite3 PRIVATE 
    SQLITE_THREADSAFE=0           # Single-threaded (no necesitamos concurrencia)
    SQLITE_OMIT_LOAD_EXTENSION    # No necesitamos extensiones dinámicas
    _WIN32_WINNT=0x0600           # Mínimo Windows Vista (compatibilidad con MinGW antiguo)
)
if(MSVC)
    target_compile_options(sqlite3 PRIVATE /W0)  # Silenciamos warnings de sqlite3.c en MSVC
else()
    target_compile_options(sqlite3 PRIVATE -w)   # Silenciamos warnings de sqlite3.c en GCC/Clang
endif()

# Creamos el ejecutable principal
add_executable(cuacker ${SOURCES} ${HEADERS})
target_link_libraries(cuacker sqlite3)

# Creamos el ejecutable de pruebas de estrés
# Excluimos src/main.cpp porque el test tiene su propio punto de entrada
file(GLOB_RECURSE CORE_SOURCES src/model/*.cpp src/storage/*.cpp)
add_executable(stress_test performance_test.cpp ${CORE_SOURCES} ${HEADERS})
target_link_libraries(stress_test sqlite3)

# Optimizaciones para máxima velocidad en el test (O3)
if(MSVC)
    target_compile_options(cuacker PRIVATE /W4 /O2)
    target_compile_options(stress_test PRIVATE /W4 /O2)
else()
    target_compile_options(cuacker PRIVATE -Wall -Wextra -O3)
    target_compile_options(stress_test PRIVATE -Wall -Wextra -O3)
endif()

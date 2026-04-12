cmake_minimum_required(VERSION 3.10)

# Nombre de tu proyecto y lenguaje
project(Cuacker LANGUAGES CXX)

# Exigimos el estándar C++11 (o superior para las listas de inicialización y otras mejoras)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadimos los directorios de cabeceras para que los #include funcionen sin modificar la ruta relativa en cada fichero
include_directories(include include/model include/storage include/cli)

# Buscamos automáticamente todos los archivos .cpp y .h en la nueva estructura de carpetas
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

# Creamos el ejecutable principal
add_executable(cuacker ${SOURCES} ${HEADERS})

# Creamos el ejecutable de pruebas de estrés (Nivel 3)
# Excluimos src/main.cpp porque el test tiene su propio punto de entrada
file(GLOB_RECURSE CORE_SOURCES src/model/*.cpp src/storage/*.cpp)
add_executable(stress_test performance_test.cpp ${CORE_SOURCES} ${HEADERS})

# Optimizaciones para máxima velocidad en el test (O3)
if(MSVC)
    target_compile_options(cuacker PRIVATE /W4 /O2)
    target_compile_options(stress_test PRIVATE /W4 /O2)
else()
    target_compile_options(cuacker PRIVATE -Wall -Wextra -O3)
    target_compile_options(stress_test PRIVATE -Wall -Wextra -O3)
endif()

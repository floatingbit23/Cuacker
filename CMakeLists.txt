cmake_minimum_required(VERSION 3.10)

# Nombre de tu proyecto y lenguaje
project(Cuacker LANGUAGES CXX)

# Exigimos el estándar C++11 (o superior para las listas de inicialización y otras mejoras)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscamos automáticamente todos los archivos .cpp y .h en la carpeta actual
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

# Creamos el ejecutable uniendo todos los archivos encontrados
# Lo llamamos "cuacker" en minúsculas para seguir la convención
add_executable(cuacker ${SOURCES} ${HEADERS})

# Mostramos información extra al compilar (opcional)
if(MSVC)
    target_compile_options(cuacker PRIVATE /W4)
else()
    target_compile_options(cuacker PRIVATE -Wall -Wextra)
endif()

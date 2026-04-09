# Cuacker 🦆

[Read in English](README_EN.md)

Motor de gestión de datos de alto rendimiento desarrollado en C++. El proyecto simula una plataforma de microblogging (como Twitter/X) centrada en la eficiencia algorítmica, utilizando estructuras de datos avanzadas para el almacenamiento, búsqueda y recuperación de "Cuacs" (equivalente a los "tweets").

## Características Principales

1. **Estructuras Híbridas**: Implementación de Árboles AVL para búsquedas ordenadas por fecha e ID, y de Tablas Hash para acceso indexado veloz.
   - Los AVL garantizan tiempos de búsqueda, inserción y borrado logarítmicos de $O(logn)$.
   - La Tabla Hash permite búsquedas por clave con coste promedio constante $O(1)$.

2. **Intérprete de Comandos**: Incluye un módulo `Interprete` capaz de parsear y ejecutar instrucciones personalizadas desde la terminal en tiempo real.

3. **Gestión de Entidades (Cuacs)**: Manejo completo de objetos complejos que incluyen IDs únicos, *timestamps* (clase `Fecha`) y contenido de texto. El Gestor de Diccionario (`DiccionarioCuacs`) coordina la redundancia entre el árbol y la tabla para ofrecer versatilidad en las consultas.

4. **Documentación Integrada**: Configuración de Doxygen lista para generar manual técnico automático.

## Construcción y Ejecución

El proyecto utiliza **CMake**, el estándar de la industria para la construcción de software en C++.

### Requisitos
- Compilador de C++ (GCC/MinGW, Clang o MSVC).
- **CMake** (v3.10 o superior).

### Opción A: Desde Visual Studio Code (Recomendado)
1. Instala la extensión **CMake Tools**.
2. Abre la carpeta del proyecto en VS Code.
3. Elige tu compilador (Kit) en la barra inferior (ej. `GCC` o `MinGW`).
4. Haz clic en el botón **Build** (Icono de engranaje) en la barra inferior.
5. Para ejecutarlo o debugear, haz clic en el icono del **Bicho (Debug)**.

### Opción B: Desde la Terminal (CLI)
Si prefieres usar la línea de comandos, sigue estos pasos:

```bash
# 1. Crea una carpeta para la compilación (mantiene el código limpio)
mkdir build
cd build

# 2. Configura el proyecto con CMake
cmake ..

# 3. Compila el ejecutable
cmake --build .
```

### Ejecución
Una vez compilado, inicia la aplicación (desde la carpeta `build`):

```bash
./cuacker.exe
```

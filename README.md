# Cuacker 🦆

[Read in English](README_EN.md)

Motor de gestión de datos de alto rendimiento desarrollado en C++. El proyecto simula una plataforma de microblogging (como Twitter/X) centrada en la eficiencia algorítmica, utilizando estructuras de datos avanzadas para el almacenamiento, búsqueda y recuperación de "Cuacs" (equivalente a los "tweets").

## Características Principales

1. **Estructuras Híbridas**: Implementación de Árboles AVL para búsquedas ordenadas por fecha e ID, y de Tablas Hash para acceso indexado veloz.
   - Los AVL garantizan tiempos de búsqueda, inserción y borrado logarítmicos de $O(logn)$.
   - La Tabla Hash permite búsquedas por clave con coste promedio constante $O(1)$.

2. **Intérprete de Comandos**: Incluye un módulo `Interprete` capaz de parsear y ejecutar instrucciones personalizadas desde la terminal en tiempo real.

3. **Gestión de Entidades (Cuacs)**: Manejo completo de objetos complejos que incluyen IDs, *timestamps* (clase `Fecha`) y contenido de texto. El Gestor de Diccionario (`DiccionarioCuacs`) coordina la redundancia entre el árbol y la tabla para ofrecer versatilidad en las consultas.

4. **Documentación Integrada**: Configuración de Doxygen lista para generar manual técnico automático en LaTeX.

## Construcción y Ejecución

El proyecto utiliza un sistema de construcción basado en `Makefile`. Según las herramientas que tengas instaladas en Windows, puedes usar:

- **Opción A (con `make` o `mingw32-make`):**
  ```bash
  make
  ```
  *(O `mingw32-make` si usas MinGW puro).*

- **Opción B (Compilación directa con `g++`):**
  Si no tienes `make` instalado, puedes compilar todo directamente con:
  ```bash
  g++ *.cpp -o cuacker.exe
  ```

### Ejecución
Una vez compilado, inicia la aplicación con:

```bash
./cuacker.exe
```

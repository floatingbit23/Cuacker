Small university project in C++ for Algorithms and Data Structure course, as a first approach to building a data management engine. 

# Cuacker 🦆

Motor de gestión de datos de alto rendimiento desarrollado en C++. El proyecto simula una plataforma de microblogging (como Twitter/X) centrada en la eficiencia algorítmica, utilizando estructuras de datos avanzadas para el almacenamiento, búsqueda y recuperación de "Cuacs" (equivalente a los "tweets").

## Características Principales

1. Estructuras Híbridas: Implementación de Árboles AVL para búsquedas ordenadas por fecha e ID, y de Tablas Hash para acceso indexado veloz.
  - Los AVL garantiza tiempos de búsqueda, inserción y borrado logarítmicos de $O(logn)$.
  - La Tabla Hash permite búsquedas por clave con coste promedio constante $O(1)$.

2. Intérprete de Comandos: Incluye un módulo Interprete capaz de parsear y ejecutar instrucciones personalizadas desde la terminal en tiempo real.

3. Gestión de Entidades (Cuacs): Manejo completo de objetos complejos que incluyen IDs, _timestamps_ (clase Fecha) y contenido de texto. El Gestor de Diccionario coordina la redundancia entre el árbol y la tabla para ofrecer versatilidad en las consultas.

4. Documentación Integrada: Configuración de Doxygen lista para generar manual técnico automático en LaTeX.

## Construcción

El proyecto utiliza un sistema de construcción basado en Makefile.

---------------------------------------------------------------------------------------------------------------------------

# Cuacker (English) 🦆

A high-performance data management engine developed in C++. This project simulates a microblogging platform (similar to Twitter/X) focused on algorithmic efficiency, utilizing advanced data structures for the storage, search, and retrieval of "Cuacs" (the platform's equivalent of "tweets").

## Key Features

1. Hybrid Data Structures: Implementation of AVL Trees for ordered searches by date and ID, combined with Hash Tables for rapid indexed access.
    - AVL Trees guarantee logarithmic search, insertion, and deletion times of $O(logn)$.
    - The Hash Table enables key-based searches with an average constant complexity of $O(1)$.

2. Command Interpreter: Features an Interprete module capable of parsing and executing custom instructions from the terminal in real-time.

3. Entity Management (Cuacs): Comprehensive handling of complex objects including unique IDs, timestamps (via the Fecha class), and text content. The Dictionary Manager (DiccionarioCuacs) coordinates redundancy between the tree and the table to provide maximum query versatility.

4. Integrated Documentation: Doxygen configuration is ready to automatically generate the technical manual in LaTeX.

## Build System

The project utilizes a Makefile-based build system for streamlined compilation.




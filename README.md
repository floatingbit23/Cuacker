# Cuacker 🦆

[Read in English](README_EN.md)

Motor de gestión de datos de alto rendimiento desarrollado en C++. El proyecto simula una plataforma de microblogging (como Twitter/X) centrada en la eficiencia algorítmica, utilizando estructuras de datos avanzadas para el almacenamiento, búsqueda y recuperación de "Cuacs" (equivalente a los "tweets").

## Características Principales

1. **Estructuras Híbridas y Escalado Dinámico**: 
   - **Tabla Hash con Rehash**: Acceso indexado $O(1)$. Implementa redimensionamiento dinámico automático (factor de carga >0.75) para garantizar que el rendimiento no degrade con millones de usuarios.
   - **Árboles AVL**: Búsquedas temporales y por ID garantizadas en $O(log n)$ mediante balanceo automático de ramas (rotaciones).
   
2. **Persistencia de Datos (CSV Engine)**:
   - Motor de serialización propio en `Persistencia.cpp` que guarda el estado del sistema en `cuacs.dat`.
   - Carga automática al inicio y guardado preventivo al salir o mediante comando `save`.

3. **Arquitectura Limpia**:
   - Separación estricta de responsabilidades:
     - `src/model`: Entidades de datos puros.
     - `src/storage`: Motores de datos e índices (Estructuras de datos).
     - `src/cli`: Capa de interacción con el usuario (Interfaz).
   - Migración completa a un sistema de construcción profesional con **CMake**.

4. **Gestión de Ciclo de Vida (CRUD)**:
   - Implementado el comando `delete` con rebalanceo real de árboles AVL y purga sincronizada en la Tabla Hash.

## Comandos Disponibles

| Comando | Acción | Complejidad |
|---|---|---|
| `mcuac` / `pcuac` | Publicar un nuevo Cuac | $O(1) + O(\log n)$ |
| `follow <usuario>` | Ver todos los cuacs de un usuario | $O(1)_{\text{avg}} + O(k)$ |
| `delete <id>` | Eliminar un mensaje permanentemente | $O(\log n)$ |
| `last <n>` | Ver los últimos 'n' mensajes del sistema | $O(\log n + k)$ |
| `date <F1> <F2>` | Ver los ensajes en un rango de fechas | $O(\log n + k)$ |
| `tag <#hashtag>` | Búsqueda por etiquetas indexadas | $O(\log n + k)$ |
| `save` | Forzar guardado en base de datos | $O(n)$ |
| `search <texto>` | Búsqueda de subcadena en mensajes | $O(n \cdot m)$ |

Siendo:
- **$n$**: Número total de Cuacs (mensajes) almacenados en el sistema.
- **$k$**: Número de elementos recuperados y mostrados (el "payload" de la respuesta).
- **$m$**: Longitud de la cadena de texto buscada (en el comando `search`).

## 📊 Benchmark de Rendimiento (Validado 300K)

Se ha verificado la robustez del sistema con un Stress Test masivo:
- **Inserción:** Sostenida gracias al **Rehash dinámico** (la tabla crece automáticamente hasta +400.000 buckets).
- **Recuperación AVL (`last`):** 4.18 ms para 100 consultas sobre 300.000 elementos.
- **Proyección 1M Cuacs:** ~4.6 ms (el crecimiento es logarítmico, solo un 10% más lento con el triple de datos).
- **Búsqueda Hash:** Acceso instantáneo a cualquier usuario entre millones.

## 🏗️ Construcción y Ejecución

### Requisitos
- Compilador C++11 o superior (GCC/MinGW, Clang).
- **CMake** (v3.10+).

### Instalación Rápida
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Tip de Ejemplo
Para probar el programa con datos precargados:
1. Renombra el archivo `cuacs_ejemplo.dat` a `cuacs.dat` en la carpeta raíz.
2. Ejecuta `./cuacker.exe`.
3. ¡Verás que el sistema restaura automáticamente 5 mensajes de bienvenida!

---
*Desarrollado para la asignatura de Algoritmos y Estructuras de Datos.*

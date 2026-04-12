# Cuacker 🦆

[Read in English](README_EN.md)

Motor de gestión de datos de alto rendimiento desarrollado en C++. El proyecto simula una plataforma de microblogging (como Twitter/X) centrada en la eficiencia algorítmica, utilizando estructuras de datos avanzadas para el almacenamiento, búsqueda y recuperación de "Cuacs" (equivalente a los "tweets").

## Características Principales

1. **Estructuras Híbridas y Escalado Dinámico**: 
   - **Tabla Hash con Rehash**: Acceso indexado $O(1)$. Implementa redimensionamiento dinámico automático (factor de carga >0.75) para garantizar que el rendimiento no degrade con millones de usuarios.
   - **Árboles AVL**: Búsquedas temporales y por ID garantizadas en $O(log n)$ mediante balanceo automático de ramas (rotaciones).
   
2. **Persistencia Profesional (SQLite Engine)**:
   - Motor de persistencia incremental basado en **SQLite 3**.
   - Uso de **Prepared Statements** para garantizar máximo rendimiento y seguridad (evitando ataques de inyección SQL).
   - Sincronización automática del contador de IDs de los cuacs entre sesiones.

3. **Arquitectura Limpia**:
   - Separación estricta de responsabilidades:
     - `src/model`: Entidades de datos puros.
     - `src/storage`: Motores de datos e índices (Estructuras de datos).
     - `src/cli`: Capa de interacción con el usuario (Interfaz).
   - Migración completa a un sistema de construcción profesional con **CMake**.

4. **Gestión de Ciclo de Vida (CRUD)**:
   - Implementado el comando `delete` con rebalanceo real de árboles AVL y purga sincronizada en la Tabla Hash.

5. **Grafo Social (Seguidores)**:
   - Tabla `seguidores` en SQLite con **clave primaria compuesta** `(seguidor, seguido)` que elimina duplicados de forma nativa.
   - Caché en RAM del grafo usando `unordered_map<string, unordered_set<string>>` para lookups $O(1)$.
   - **Timeline personalizado**: el comando `last` muestra solo los cuacs de los usuarios seguidos + los propios cuando hay sesión activa.
   - Sistema de sesión ligero con `login`/`logout` — sin login, el sistema funciona en modo global (retrocompatible).

## Comandos Disponibles

### Publicación y Búsqueda

| Comando | Operación | Complejidad | Nota |
| :--- | :--- | :--- | :--- |
| `mcuac <usr> <fecha> <msg>` | Publicar mensaje manual | $O(1) + O(\log n)$ | RAM + SQLite |
| `pcuac <usr> <fecha> <num>` | Publicar mensaje predefinido | $O(1) + O(\log n)$ | RAM + SQLite |
| `follow <usuario>` | Seguir a un usuario y ver sus cuacs | $O(1)_{\text{avg}}$ | Hash + SQLite (grafo) |
| `last <N>` | Últimos N cuacs (global o timeline personal) | $O(n)^*$ / $O(\log n)$ | AVL filtrado o global |
| `date <f1> <f2>` | Cuacs en rango de fechas | $O(\log n + k)$ | AVL (RAM) |
| `tag <#hashtag>` | Búsqueda por hashtag | $O(\log n + k)$ | Índice hashtags |
| `search <texto>` | Búsqueda de texto libre | $O(n \cdot m)$ | AVL inorden |
| `delete <id>` | Borrar cuac por ID | $O(\log n)$ | RAM + SQL DELETE |
| `check` | Verificar integridad de la BBDD | $O(1)$ | PRAGMA integrity |

### Grafo Social (requiere `login`)

| Comando | Operación |
| :--- | :--- |
| `login <usuario>` | Iniciar sesión (carga grafo en RAM desde SQLite) |
| `logout` | Cerrar sesión (vuelve al modo global) |
| `whoami` | Mostrar el usuario con sesión activa |
| `unfollow <usuario>` | Dejar de seguir a un usuario |
| `following` | Ver la lista de usuarios que sigues |
| `followers` | Ver la lista de usuarios que te siguen |

> [!NOTE]
> $^*$ `last` con login activo tiene complejidad $O(n)$ porque recorre todo el AVL aplicando el filtro. El filtrado por `unordered_set` es $O(1)$ por nodo, por lo que en la práctica es extremadamente rápido.

Siendo:
- **$n$**: Número total de Cuacs almacenados en el sistema.
- **$k$**: Número de elementos recuperados y mostrados.
- **$m$**: Longitud de la cadena buscada (comando `search`).

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

### Gestión de la Base de Datos
- Al primer arranque, el sistema crea automáticamente el archivo `cuacs.db`.
- Los datos se guardan en tiempo real sin necesidad de comandos manuales.
- Puedes usar el comando `check` para ejecutar un `PRAGMA integrity_check` sobre la base de datos almacenada.

---
*Desarrollado para la asignatura de **Algoritmos y Estructuras de Datos**.*

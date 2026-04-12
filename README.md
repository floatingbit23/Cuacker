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

## Comandos Disponibles

| Comando | Acción | Complejidad |
|---|---|---|
| `mcuac` / `pcuac` | Publicar un nuevo Cuac | $O(1) + O(\log n) + O(1)_{\text{db}}$ |
| `follow <usuario>` | Ver todos los cuacs de un usuario | $O(1)_{\text{avg}} + O(k)$ |
| `delete <id>` | Eliminar un mensaje permanentemente | $O(\log n) + O(1)_{\text{db}}$ |
| `last <n>` | Ver los últimos 'n' mensajes del sistema | $O(\log n + k)$ |
| `date <F1> <F2>` | Ver los ensajes en un rango de fechas | $O(\log n + k)$ |
| `tag <#hashtag>` | Búsqueda por etiquetas indexadas | $O(\log n + k)$ |
| `check` | Verificar integridad de la base de datos | $O(1)$ |
| `search <texto>` | Búsqueda de texto (subcadena) en mensajes | $O(n \cdot m)$ |

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

### Gestión de la Base de Datos
- Al primer arranque, el sistema crea automáticamente el archivo `cuacs.db`.
- Los datos se guardan en tiempo real sin necesidad de comandos manuales.
- Puedes usar el comando `check` para ejecutar un `PRAGMA integrity_check` sobre la base de datos almacenada.

---
*Desarrollado para la asignatura de **Algoritmos y Estructuras de Datos**.*

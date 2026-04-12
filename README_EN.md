# Cuacker (English) 🦆

[Leer en Español](README.md)

A high-performance data management engine developed in C++. This project simulates a microblogging platform (similar to Twitter/X) focused on algorithmic efficiency, utilizing advanced data structures for the storage, search, and retrieval of "Cuacs" (the platform's equivalent of "tweets").

## Key Features

1. **Hybrid Structures & Dynamic Scaling**: 
   - **Hash Table with Rehash**: $O(1)$ indexed access. Features automatic dynamic resizing (load factor >0.75) to maintain performance at scale.
   - **AVL Trees**: Guaranteed $O(log n)$ temporal and ID searches via self-balancing tree branches (rotations).
   
2. **Data Persistence (CSV Engine)**:
   - Custom serialization engine in `Persistencia.cpp` that tracks system state in `cuacs.dat`.
   - Automatic background loading on startup and safe-saving on exit or via the `save` command.

3. **Clean Architecture Layout**:
   - Strict separation of concerns (SoC):
     - `src/model`: Pure data entities.
     - `src/storage`: Data engines and indexing logic.
     - `src/cli`: User interaction layer.
   - Professional build system powered by **CMake**.

4. **Full CRUD Lifecycle**:
   - Implemented real `delete` functionality with AVL tree rebalancing and synchronized Hash Table purging.

## Available Commands

| Command | Action | Complexity |
|---|---|---|
| `mcuac` / `pcuac` | Post a new Cuac | $O(1) + O(\log n)$ |
| `follow <user>` | View all cuacs from a user | $O(1)_{\text{avg}} + O(k)$ |
| `delete <id>` | Permanently delete a message | $O(\log n)$ |
| `last <n>` | View the latest 'n' messages | $O(\log n + k)$ |
| `date <F1> <F2>` | Messages in a date range | $O(\log n + k)$ |
| `tag <#hashtag>` | Search via indexed tags | $O(\log n + k)$ |
| `save` | Force database persistence | $O(n)$ |
| `search <text>` | Search substring in messages | $O(n \cdot m)$ |

With:
- **$n$**: Total number of Cuacs (messages) stored in the system.
- **$k$**: Number of elements retrieved and displayed ("payload" of the response).
- **$m$**: Length of the search string (for the `search` command).

## 📊 Performance Benchmarks (300K Validated)

System robustness has been verified with a massive Stress Test:
- **Insertion:** Sustained via **Dynamic Rehash** (table auto-scales up to +400,000 buckets).
- **AVL Retrieval (`last`):** 4.18 ms for 100 queries over 300,000 elements ($O(\log n)$).
- **1M Cuacs Projection:** ~4.6 ms (logarithmic growth, only 10% slower with 3x the data).
- **Hash Search:** Instant access to any user among millions ($O(1)$).

## 🏗️ Build and Run

### Requirements
- C++11 Compiler or higher (GCC/MinGW, Clang).
- **CMake** (v3.10+).

### Quick Start
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 💡 Pro Tip
To test the program with pre-loaded data:
1. Rename the `cuacs_ejemplo.dat` file to `cuacs.dat` in the root folder.
2. Run `./cuacker.exe`.
3. You will see the system automatically restoring the sample welcome messages!

---
*Developed for Algorithms and Data Structures.*

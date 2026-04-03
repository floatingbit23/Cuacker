# Cuacker (English) 🦆

[Leer en Español](README.md)

Small university project in C++ for Algorithms and Data Structure course, as a first approach to building a data management engine.

A high-performance data management engine developed in C++. This project simulates a microblogging platform (similar to Twitter/X) focused on algorithmic efficiency, utilizing advanced data structures for the storage, search, and retrieval of "Cuacs" (the platform's equivalent of "tweets").

## Key Features

1. **Hybrid Data Structures**: Implementation of AVL Trees for ordered searches by date and ID, combined with Hash Tables for rapid indexed access.
   - AVL Trees guarantee logarithmic search, insertion, and deletion times of $O(logn)$.
   - The Hash Table enables key-based searches with an average constant complexity of $O(1)$.

2. **Command Interpreter**: Features an `Interprete` module capable of parsing and executing custom instructions from the terminal in real-time.

3. **Entity Management (Cuacs)**: Comprehensive handling of complex objects including unique IDs, timestamps (via the `Fecha` class), and text content. The Dictionary Manager (`DiccionarioCuacs`) coordinates redundancy between the tree and the table to provide maximum query versatility.

4. **Integrated Documentation**: Doxygen configuration is ready to automatically generate the technical manual in LaTeX.

## Build System and Execution

The project utilizes a `Makefile`-based build system. Depending on your Windows environment, you can use:

- **Option A (with `make` or `mingw32-make`):**
  ```bash
  make
  ```
  *(Or `mingw32-make` if you're using plain MinGW).*

- **Option B (Direct compilation with `g++`):**
  If you don't have `make` installed, you can compile everything directly with:
  ```bash
  g++ *.cpp -o cuacker.exe
  ```

### Execution
Once compiled, start the application with:

```bash
./cuacker.exe
```

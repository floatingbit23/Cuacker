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

4. **Integrated Documentation**: Doxygen configuration is ready to automatically generate the technical manual.

## Build System and Execution

The project uses **CMake**, the industry standard for C++ software builds.

### Requirements
- C++ Compiler (GCC/MinGW, Clang, or MSVC).
- **CMake** (v3.10 or higher).

### Option A: Via Visual Studio Code/Antigravity (Recommended)
1. Install the **CMake Tools** extension.
2. Open the project folder in VS Code.
3. Select your compiler (Kit) from the bottom status bar (e.g., `GCC` or `MinGW`).
4. Click the **Build** (Gear icon) button in the bottom status bar.
5. To run or start debugging, click the **Insect (Debug)** icon.

### Option B: Via Command Line (CLI)
If you prefer building from the terminal, follow these steps:

```bash
# 1. Create a build directory (keeps source clean)
mkdir build
cd build

# 2. Configure the project with CMake
cmake ..

# 3. Compile the executable
cmake --build .
```

### Execution
Once compiled, start the application (from the `build` directory):

```bash
./cuacker.exe
```

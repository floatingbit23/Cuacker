# Cuacker (English) 🦆

[Leer en Español](README.md)

A high-performance data management engine developed in C++. This project simulates a microblogging platform (similar to Twitter/X) focused on algorithmic efficiency, utilizing advanced data structures for the storage, search, and retrieval of "Cuacs" (the platform's equivalent of "tweets").

## Key Features

1. **Hybrid Structures & Dynamic Scaling**: 
   - **Hash Table with Rehash**: $O(1)$ indexed access. Features automatic dynamic resizing (load factor >0.75) to maintain performance at scale.
   - **AVL Trees**: Guaranteed $O(log n)$ temporal and ID searches via self-balancing tree branches (rotations).
   
2. **Professional Persistence (SQLite Engine)**:
   - Incremental persistence engine powered by **SQLite 3**.
   - Uses **Prepared Statements** to ensure peak performance and security (protecting against _SQL Injection_ attacks).
   - Automatic cuac's ID counter synchronization across sessions.

3. **Clean Architecture Layout**:
   - Strict separation of concerns (SoC):
     - `src/model`: Pure data entities.
     - `src/storage`: Data engines and indexing logic.
     - `src/cli`: User interaction layer.
   - Professional build system powered by **CMake**.

4. **Full CRUD Lifecycle**:
   - Implemented real `delete` functionality with AVL tree rebalancing and synchronized Hash Table purging.

5. **Social Graph (Followers)**:
   - `seguidores` table in SQLite with a **composite primary key** `(follower, followed)` that natively prevents duplicates.
   - In-RAM graph cache using `unordered_map<string, unordered_set<string>>` for $O(1)$ lookups.
   - **Personalised timeline**: the `last` command shows only cuacs from followed users + your own when a session is active.
   - Lightweight session system via `login`/`logout` — without login, the system runs in global mode (fully backwards-compatible).

## Available Commands

### Posting & Search

| Command | Operation | Complexity | Note |
| :--- | :--- | :--- | :--- |
| `mcuac <usr> <date> <msg>` | Post a manual message | $O(1) + O(\log n)$ | RAM + SQLite |
| `pcuac <usr> <date> <num>` | Post a predefined message | $O(1) + O(\log n)$ | RAM + SQLite |
| `follow <user>` | Follow a user and view their cuacs | $O(1)_{\text{avg}}$ | Hash + SQLite (graph) |
| `last <N>` | Latest N cuacs (global or personal timeline) | $O(n)^*$ / $O(\log n)$ | Filtered or global AVL |
| `date <f1> <f2>` | Cuacs in a date range | $O(\log n + k)$ | AVL (RAM) |
| `tag <#hashtag>` | Search by hashtag | $O(\log n + k)$ | Hashtag index |
| `search <text>` | Full-text search | $O(n \cdot m)$ | AVL in-order |
| `delete <id>` | Delete cuac by ID | $O(\log n)$ | RAM + SQL DELETE |
| `check` | Database integrity check | $O(1)$ | PRAGMA integrity |

### Social Graph (requires `login`)

| Command | Operation |
| :--- | :--- |
| `login <user>` | Start a session (loads graph from SQLite into RAM) |
| `logout` | End session (reverts to global mode) |
| `whoami` | Show the active session user |
| `unfollow <user>` | Unfollow a user |
| `following` | View the list of users you follow |
| `followers` | View the list of users who follow you |

> [!NOTE]
> $^*$ `last` with an active session has $O(n)$ complexity as it traverses the full AVL applying the filter. The filter check via `unordered_set` is $O(1)$ per node, making it extremely fast in practice.

With:
- **$n$**: Total number of Cuacs stored in the system.
- **$k$**: Number of elements retrieved and displayed.
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

### Database Management
- On the first run, the system automatically creates the `cuacs.db` file.
- Data is saved in real-time without the need for manual commands.
- You can use the `check` command to run a `PRAGMA integrity_check` on the stored database.

---
*Developed for **Algorithms and Data Structures** subject.*

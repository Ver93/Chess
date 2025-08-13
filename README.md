# ♟️ Rookie Chess Engine

**Rookie** — By Name, Grandmaster By Nature.

A UCI-compatible chess engine written in C++23.

---

## Features

- Bitboard-based move generation
- UCI protocol support
- Perft testing
- Castling, en passant, promotion logic
- Modular architecture

---

## Build Instructions

### Prerequisites
- CMake ≥ 3.16
- Visual Studio 2022 (or GCC/Clang for Linux/macOS)

### Build (Windows)
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel
```

## 🌐 Play Against Botomir on Lichess!

**Botomir** - is powered by the Rookie chess engine and battles players and bots alike on [Lichess.org](https://lichess.org/@/Botomir).

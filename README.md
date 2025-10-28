# Chess Engine

A complete chess engine written in C++17 that supports both command-line gameplay and the Universal Chess Interface (UCI) protocol for integration with chess GUIs.

## Features

- **Complete Chess Implementation**: Full rule support including castling, en passant, and pawn promotion
- **Legal Move Generation**: Generates all legal moves with proper check detection
- **Alpha-Beta Search**: Minimax search with alpha-beta pruning for intelligent gameplay
- **Position Evaluation**: Material and positional evaluation using piece-square tables
- **Multiple Interfaces**:
  - Command-line interface for human vs engine games
  - UCI protocol support for chess GUI integration
- **Comprehensive Testing**: Full test suite ensuring correctness of all components

## Building the Engine

### Prerequisites

- **C++17 compatible compiler** (GCC, Clang, or MSVC)
- **CMake** 3.10 or higher

### macOS/Linux Build Instructions

```bash
# Clone the repository
git clone https://github.com/tegveerbhatti/chess-engine.git
cd chess-engine

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run tests (optional but recommended)
ctest --verbose
```

### Windows Build Instructions

```bash
# Clone the repository
git clone https://github.com/tegveerbhatti/chess-engine.git
cd chess-engine

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build . --config Release

# Run tests (optional but recommended)
ctest --verbose
```

## Usage

### Command-Line Mode

Play directly against the engine in your terminal:

```bash
./chess_engine
```

- You play as White
- Enter moves in coordinate notation (e.g., `e2e4`, `g1f3`)
- The engine will respond with its move
- The game continues until checkmate or stalemate

### UCI Mode

Connect the engine to any UCI-compatible chess GUI:

```bash
./chess_engine --uci
```

#### Setting up with a GUI:
1. Launch your chess GUI
2. Go to engine settings
3. Add new UCI engine
4. Point to the `chess_engine` executable in your `build` directory
5. The engine will now be available for games and analysis

## Engine Strength

This engine implements:
- **Material evaluation** with standard piece values
- **Positional evaluation** using piece-square tables
- **Alpha-beta pruning** with move ordering for efficient search
- **Depth-4 search** by default (configurable in UCI mode)

The engine plays at an intermediate level and provides a good challenge for casual players while being educational for those interested in chess programming.

## Architecture

The engine is built with a modular architecture:

- **Board Representation**: 8x8 array with efficient piece encoding
- **Move Generation**: Pseudo-legal generation followed by legality filtering
- **Search Algorithm**: Minimax with alpha-beta pruning
- **Evaluation**: Material + positional evaluation
- **Interface Layer**: Separate CLI and UCI implementations

## Testing

The engine includes comprehensive tests covering:
- FEN parsing and generation
- Move generation correctness
- Make/unmake move reversibility
- Evaluation function sanity
- Search algorithm tactical awareness

Run all tests with:
```bash
cd build
ctest --verbose
```

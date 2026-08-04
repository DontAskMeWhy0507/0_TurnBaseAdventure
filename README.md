# TurnBaseAdventure

A turn-based adventure game built with C++11.

## Project Structure

```
TurnBaseAdventure/
├── CMakeLists.txt      # Build configuration (CMake 3.10+, C++11)
├── main.cpp            # Application entry point
├── include/            # Public headers
│   └── Game.hpp        # Core Game class
├── src/                # Source files
│   └── Game.cpp        # Game implementation
├── data/               # Runtime data & save files
├── tests/              # Unit tests
└── README.md
```

## Requirements

- **CMake** >= 3.10
- **C++11** compatible compiler (GCC 4.8+, Clang 3.3+, MSVC 2015+)

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

On Windows with Visual Studio:

```bash
cmake -B build
cmake --build build --config Debug
```

## Run

```bash
./build/TurnBaseAdventure        # Linux/macOS
.\build\Debug\TurnBaseAdventure  # Windows (MSVC)
```

## Development Status

- [x] Project skeleton & build system
- [ ] Game loop
- [ ] Entity / component system
- [ ] Combat system
- [ ] Save / load
- [ ] Rendering

## License

MIT

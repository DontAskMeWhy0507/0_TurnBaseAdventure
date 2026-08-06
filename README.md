# TurnBaseAdventure

A turn-based adventure game built with C++11.

## Project Structure

```
TurnBaseAdventure/
├── CMakeLists.txt      # Build configuration (CMake 3.10+, C++11)
├── main.cpp            # Application entry point
├── include/            # Public headers
│   ├── Character.hpp   # Base character interface
│   ├── GameApp.hpp     # Main game application class
│   ├── InputHandler.hpp# Input handling logic
│   ├── Mage.hpp        # Mage character class
│   ├── MenuController.hpp # Menu management
│   └── Warrior.hpp     # Warrior character class
├── src/                # Source files
│   ├── Character.cpp   # Base character implementation
│   ├── GameApp.cpp     # Game application implementation
│   ├── InputHandler.cpp# Input handling implementation
│   ├── Mage.cpp        # Mage behavior implementation
│   ├── MenuController.cpp # Menu controller implementation
│   └── Warrior.cpp     # Warrior behavior implementation
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

## Test

```bash
./build/test_oop               # Linux/macOS
.\build\Debug\test_oop.exe   # Windows (MSVC)
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

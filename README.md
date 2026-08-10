# TurnBaseAdventure

Console-based turn-based adventure game written in C++11.

## Implemented features

- Character roster with Warrior/Mage CRUD, validation, ID search and
  case-insensitive partial-name search.
- Team creation, rename, deletion, ordered membership, roster-ID validation,
  duplicate prevention and five-member limit.
- Turn-based battles with HP/mana reset, alternating turns, dead-character
  skipping, target validation, runtime polymorphism and winner detection.
- Character and team persistence through `data/characters.txt` and
  `data/teams.txt`.
- Console menus for Roster, Team, Battle, Save/Load and Exit.
- Invalid input, malformed rows, missing files and file I/O failures are
  reported without crashing.

## Architecture

There is one canonical definition for each entity:

- `include/Character.h` defines abstract `Character`, `Warrior` and `Mage`.
- `include/Team.hpp` defines `Team`.
- `Character.hpp`, `Warrior.hpp`, `Mage.hpp` and `Team.h` are compatibility
  wrappers; they do not define duplicate classes.
- `Team` stores roster IDs for team management and non-owning character
  pointers for battle execution.
- `GameApp` owns `CharacterRoster`, `TeamManager` and `BattleEngine`, then
  injects them into `MenuController`.

This single-model design prevents the original ODR/ABI conflict between the
battle and roster/team modules.

## Data formats

`data/characters.txt`:

```text
WARRIOR,id,name,maxHp,attackPower
MAGE,id,name,maxHp,maxMana,spellDamage,manaCost,fallbackDamage
```

`data/teams.txt`:

```text
teamId|teamName|characterId1,characterId2,...
```

Malformed rows are skipped with a diagnostic message. Current HP, current
mana and an active battle are intentionally not persisted.

## Requirements

- CMake 3.10 or newer
- C++11-compatible compiler

## Build

From the project root:

```powershell
cmake -S . -B build2 -DCMAKE_BUILD_TYPE=Debug
cmake --build build2 --config Debug
```

## Run the application

```powershell
.\build2\TurnBaseAdventure.exe
```

The main menu provides:

1. Character Roster Management
2. Team Management
3. Battle
4. Save & Load Data
0. Exit

Data is loaded during startup and saved before application shutdown.

## Run tests

Run the complete CTest suite:

```powershell
ctest --test-dir build2 --output-on-failure
```

The suite contains:

- `team_tests`: team rules and persistence;
- `battle_engine_tests`: battle behavior TC-06 through TC-15;
- `oop_tests`: Character/Warrior/Mage behavior;
- `acceptance_tests`: explicitly runs TC-01 through TC-18 and prints an
  individual `PASS` line for every case.

To run all 18 acceptance cases directly:

```powershell
.\build2\acceptance_tests.exe
```

Expected final output:

```text
TC-01 PASS
...
TC-18 PASS
ALL 18 ACCEPTANCE TESTS PASSED
```

## Project structure

```text
TurnBaseAdventure/
├── CMakeLists.txt
├── main.cpp
├── include/              # Public headers
├── src/                  # Application and domain implementations
├── data/                 # Character/team persistence files
└── tests/                # Unit, battle and acceptance tests
```

## License

MIT

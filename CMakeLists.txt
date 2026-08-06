cmake_minimum_required(VERSION 3.10)
project(TurnBaseAdventure VERSION 0.1.0 LANGUAGES CXX)

# C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect sources
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.c"
)

# Main executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Test executable for BattleEngine
add_executable(test_battle_engine
    tests/test_battle_engine.cpp
    ${SOURCES}
)
target_include_directories(test_battle_engine PRIVATE include)
if(MSVC)
    target_compile_options(test_battle_engine PRIVATE /W4)
else()
    target_compile_options(test_battle_engine PRIVATE -Wall -Wextra -pedantic)
endif()


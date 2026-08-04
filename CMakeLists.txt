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

# Optional: Tests (uncomment when adding test framework)
# enable_testing()
# add_subdirectory(tests)

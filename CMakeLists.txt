cmake_minimum_required (VERSION 3.14)

project (insound)
set (CMAKE_CXX_STANDARD 20)

# Include submodule libraries
add_subdirectory (lib)
add_subdirectory (insound)

# Insound server driver
add_executable(${PROJECT_NAME}-server main.cpp)
target_link_libraries(${PROJECT_NAME}-server insound)

enable_testing()

# Tests
add_subdirectory (tests)

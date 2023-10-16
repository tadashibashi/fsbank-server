cmake_minimum_required (VERSION 3.14)

project (fsbank-server)
set (CMAKE_CXX_STANDARD 20)

# Include submodule libraries
add_subdirectory(lib)

# FSBank server setup
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE fmod fsbank Crow)

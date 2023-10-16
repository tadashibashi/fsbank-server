cmake_minimum_required (VERSION 3.14)

project (fsbank-server)
set (CMAKE_CXX_STANDARD 20)

# Include submodule libraries
add_subdirectory (lib)

# FSBank server setup
file (GLOB_RECURSE FSBANK_SERVER_SRC src/*.cpp)
add_executable (${PROJECT_NAME} ${FSBANK_SERVER_SRC})
target_link_libraries (${PROJECT_NAME} PRIVATE fmod fsbank Crow)

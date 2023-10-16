cmake_minimum_required (VERSION 3.14)

project (fsbank-server)
set (CMAKE_CXX_STANDARD 20)

# Include submodule libraries
add_subdirectory (lib)

set (CROW_FEATURES compression)

# FSBank server setup
file (GLOB_RECURSE FSBANK_SERVER_SRC src/*.cpp)
add_executable (${PROJECT_NAME} ${FSBANK_SERVER_SRC})
target_link_libraries (${PROJECT_NAME} PRIVATE fmod fsbank z Crow)
target_compile_definitions(${PROJECT_NAME} PRIVATE "CROW_ENABLE_COMPRESSION=1")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/Crow/include)

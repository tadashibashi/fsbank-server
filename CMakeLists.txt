cmake_minimum_required (VERSION 3.14)
project(insound)

# C++ 20
set(CMAKE_CXX_STANDARD 20)

# Set module path
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

include(ConfigMessages)
include(FormatBuildType)

# Include submodule libraries
add_subdirectory(lib)
add_subdirectory(insound)

# Insound server driver
add_executable(${PROJECT_NAME}-server main.cpp)
target_link_libraries(${PROJECT_NAME}-server insound)

# Installation
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install(TARGETS ${PROJECT_NAME}-server
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

enable_testing()

# Tests
add_subdirectory(tests)

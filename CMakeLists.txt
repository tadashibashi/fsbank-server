cmake_minimum_required (VERSION 3.14)

project (insound)
set (CMAKE_CXX_STANDARD 20)

# Format cmake build type
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER)
if (${BUILD_TYPE_UPPER} MATCHES "DEBUG")
    set (CMAKE_BUILD_TYPE "Debug")
elseif(${BUILD_TYPE_UPPER} MATCHES "RELEASE")
    set (CMAKE_BUILD_TYPE "Release")
elseif(${BUILD_TYPE_UPPER} MATCHES "RELWITHDEBINFO")
    set (CMAKE_BUILD_TYPE "RelWithDebInfo")
elseif(${BUILD_TYPE_UPPER} MATCHES "MINSIZEREL")
    set (CMAKE_BUILD_TYPE "MinSizeRel")
else()
    message(FATAL_ERROR "Build type ${CMAKE_BUILD_TYPE} not recognized.")
endif()

# Include submodule libraries
add_subdirectory (lib)
add_subdirectory (insound)

# Insound server driver
add_executable(${PROJECT_NAME}-server main.cpp)
target_link_libraries(${PROJECT_NAME}-server insound)

enable_testing()

# Tests
add_subdirectory (tests)

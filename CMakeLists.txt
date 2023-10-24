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

    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set (FMOD_LIBS
            ${CMAKE_SOURCE_DIR}/lib/fmod/lib/macos/libfmod.dylib
            ${CMAKE_SOURCE_DIR}/lib/fsbank/lib/macos/libfsbank.dylib
        )
    else()
        set (FMOD_LIBS
            ${CMAKE_SOURCE_DIR}/lib/fmod/lib/linux/libfmod.so.13
            ${CMAKE_SOURCE_DIR}/lib/fsbank/lib/linux/libfsbank.so.13
            ${CMAKE_SOURCE_DIR}/lib/fsbank/lib/linux/libfsbvorbis.so
            ${CMAKE_SOURCE_DIR}/lib/fsbank/lib/linux/libopus.so.0
        )
    endif()

    install (DIRECTORY ${CMAKE_SOURCE_DIR}/public/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/public)
    install (FILES ${FMOD_LIBS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()

enable_testing()

# Tests
add_subdirectory(tests)

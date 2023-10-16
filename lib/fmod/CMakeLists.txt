# ---------------------------------------------------------------------------- #
# Targetable library for fmod with includes and libs set
# Example:
#   target_link_libraries( ${PROJECT_NAME} PRIVATE fmod )
# Platforms supported:
# - WebAssembly
# Make sure to add the Emscripten.cmake toolchain file in the configuration
# step, e.g.:
# cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<path/to/Emscripten.cmake>
project(fmod)

if (EMSCRIPTEN)
    # Flags necessary for FMOD Module
    set (CMAKE_EXE_LINKER_FLAGS "-sEXPORTED_RUNTIME_METHODS=cwrap,setValue,getValue")

    add_library(${PROJECT_NAME} INTERFACE)

    target_link_libraries(${PROJECT_NAME} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/fmod_wasm.a)

    target_include_directories(${PROJECT_NAME} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
else()

    add_library(${PROJECT_NAME} INTERFACE)
    
    target_include_directories(${PROJECT_NAME} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
        
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")                  # mac
        target_link_libraries(${PROJECT_NAME} INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/macos/libfmod.dylib
        )
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")                                 # non-apple unix (linux)
        target_link_libraries(${PROJECT_NAME} INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/libfmod.so.13
        )
    else()
        message(FATAL_ERROR "Platform not supported")
    endif()

endif()

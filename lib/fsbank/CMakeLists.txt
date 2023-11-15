# ---------------------------------------------------------------------------- #
# Targetable library for fsbank with includes and libs set
# Example:
#   target_link_libraries( ${PROJECT_NAME} PRIVATE fsbank )
# Platforms supported:
# - macOS arm64, x86_64
# - Linux x86_64
project(fsbank)

if (NOT EMSCRIPTEN)
    add_library(${PROJECT_NAME} INTERFACE)

    # Get libraries
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        # macOS
        set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/macos")
        
        if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set (FSB_LIBS
                ${LIB_DIR}/libfsbankL.dylib
                ${CMAKE_CURRENT_SOURCE_DIR}/../fmod/lib/macos/libfmodL.dylib
            )
        else()
            set (FSB_LIBS
                ${LIB_DIR}/libfsbank.dylib
                ${CMAKE_CURRENT_SOURCE_DIR}/../fmod/lib/macos/libfmod.dylib
            )
        endif()

        if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
            message (WARNING "Libraries FSB Vorbis and Opus are not available for macOS arm64, please do not convert to those formats")
        else()
            set (FSB_LIBS ${FSB_LIBS}
                ${LIB_DIR}/libfsbvorbis.dylib
                ${LIB_DIR}/libopus.dylib
            )
        endif()

    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        # linux
        set (LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux")
        set (FSB_LIBS
            ${LIB_DIR}/libfsbvorbis.so
            ${LIB_DIR}/libopus.so.0
        )
        
        if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(FSB_LIBS ${FSB_LIBS}
                ${LIB_DIR}/libfsbankL.so.13
                ${CMAKE_CURRENT_SOURCE_DIR}/../fmod/lib/linux/libfmodL.so.13
            )
        else()
            set(FSB_LIBS ${FSB_LIBS}
                ${LIB_DIR}/libfsbank.so.13
                ${CMAKE_CURRENT_SOURCE_DIR}/../fmod/lib/linux/libfmod.so.13
            )
        endif()
    else()
        # Platform not supported
        message("Error, this platform '${CMAKE_SYSTEM_NAME}', is not supported.")
    endif()

    target_link_libraries(${PROJECT_NAME} INTERFACE ${FSB_LIBS})
    target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

endif()

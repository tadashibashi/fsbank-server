project(insound-server)
set (CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE SERVER_SRC ./*.cpp)

add_executable(${PROJECT_NAME}
    ${SERVER_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE
        insound-core)

# Installation
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR
    ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )

    # Install templates directory
    install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/templates
        DESTINATION ${CMAKE_INSTALL_PREFIX}/templates)

    # Copy FMOD libraries to install folder
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

    install (FILES ${FMOD_LIBS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

endif()

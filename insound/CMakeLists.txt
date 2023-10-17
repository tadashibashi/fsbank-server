project (insound)

set (CROW_FEATURES compression)
list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib/Crow/cmake)

find_package(asio REQUIRED)

# FSBank server setup
file (GLOB_RECURSE FSBANK_SERVER_SRC ./*.cpp)
add_library (${PROJECT_NAME} STATIC ${FSBANK_SERVER_SRC})
target_link_libraries (${PROJECT_NAME} PUBLIC fmod fsbank Poco::JWT z asio::asio)
target_compile_definitions(${PROJECT_NAME} PRIVATE "CROW_ENABLE_COMPRESSION=1")
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib/Crow/include
    PUBLIC ${CMAKE_SOURCE_DIR}
)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/definitions.h)

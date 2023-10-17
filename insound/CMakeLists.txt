project (insound)

# Enable zlib compression
set (CROW_FEATURES compression)

# Added to use Crow's FindAsio.cmake
list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/lib/Crow/cmake)

# Asio is required by Crow
find_package(asio REQUIRED)

# FSBank server setup
file (GLOB_RECURSE FSBANK_SERVER_SRC ./*.cpp)
add_library (${PROJECT_NAME} STATIC ${FSBANK_SERVER_SRC})
target_link_libraries (${PROJECT_NAME} PUBLIC fmod fsbank Poco::JWT z asio::asio)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib/Crow/include
    PUBLIC ${CMAKE_SOURCE_DIR}
)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h)

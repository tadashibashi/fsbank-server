project (insound)

# Enable zlib compression
set (CROW_FEATURES compression)

# Insound server setup
file (GLOB_RECURSE INSOUND_SRC ./*.cpp ./*.h ./*.hpp)
add_library (${PROJECT_NAME} STATIC ${INSOUND_SRC})

target_link_libraries (${PROJECT_NAME} PUBLIC
    fmod fsbank Poco::JWT Poco::Crypto z glaze::glaze curl
    mongocxx_shared spdlog::spdlog Crow bcrypt jwt-cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fmt.hpp
    )

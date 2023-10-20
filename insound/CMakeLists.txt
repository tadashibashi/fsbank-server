project (insound)

# Enable zlib compression
set (CROW_FEATURES compression)

# Insound server setup
file (GLOB_RECURSE INSOUND_SRC ./*.cpp ./*.h ./*.hpp)
add_library (${PROJECT_NAME} STATIC ${INSOUND_SRC})

target_link_libraries (${PROJECT_NAME} PUBLIC
    fmod fsbank Poco::JWT Poco::Net Poco::NetSSL Poco::Crypto z glaze::glaze
    mongocxx_shared fmt spdlog::spdlog Crow
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h
)

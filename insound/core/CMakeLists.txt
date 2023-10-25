project (insound-core)

# Enable zlib compression
set (CROW_FEATURES compression)

# Insound server setup
file (GLOB_RECURSE INSOUND_SRC ./*.cpp ./*.h ./*.hpp)
add_library (${PROJECT_NAME} STATIC ${INSOUND_SRC})

find_package(ZLIB REQUIRED)

target_link_libraries (${PROJECT_NAME} PUBLIC
    fmod fsbank ZLIB::ZLIB glaze::glaze curl Crow bcrypt jwt-cpp mongocxx_shared
    spdlog::spdlog
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}
)

if (CMAKE_INSTALL_PREFIX)
    set (PUBLIC_DIR ${CMAKE_INSTALL_PREFIX}/public)
else()
    set (PUBLIC_DIR ${CMAKE_SOURCE_DIR}/public)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fmt.hpp
    )

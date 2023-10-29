project (insound-core)

# Enable zlib compression
set (CROW_FEATURES compression)

# Insound server setup
file (GLOB_RECURSE INSOUND_SRC ./*.cpp ./*.h ./*.hpp)
add_library (${PROJECT_NAME} STATIC ${INSOUND_SRC})

# Find external dependencies not provided by this repo
find_package(ZLIB REQUIRED)

set (SERVICE_COMPONENTS s3)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

# Link libraries
target_link_libraries (${PROJECT_NAME} PUBLIC
    fmod fsbank glaze::glaze curl Crow bcrypt jwt-cpp zip
    mongocxx_shared spdlog::spdlog
    ${AWSSDK_LINK_LIBRARIES}  ZLIB::ZLIB
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}
)

if (INSOUND_INSTALL_PREFIX)
    set (STATIC_DIR ${INSOUND_INSTALL_PREFIX}/static)
else()
    set (STATIC_DIR ${CMAKE_SOURCE_DIR}/static)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/definitions.h
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fmt.hpp
    )

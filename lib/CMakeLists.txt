set (ALLOW_DUPLICATE_CUSTOM_TARGETS ON)

add_subdirectory (bcrypt)
add_subdirectory (Catch2)

set (CROW_BUILD_TESTS OFF)
set (CROW_BUILD_EXAMPLES OFF)
add_subdirectory (Crow)
add_subdirectory (fmod)
add_subdirectory (fsbank)
add_subdirectory (glaze)
add_subdirectory (jwt-cpp)

set (MONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX OFF)
add_subdirectory (mongo-cxx-driver)

set (SPDLOG_BUILD_SHARED OFF)
set (BUILD_SHARED_LIBS OFF)
add_subdirectory (spdlog)
add_subdirectory (zip)

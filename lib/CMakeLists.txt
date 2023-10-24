add_subdirectory (bcrypt)
add_subdirectory (Catch2)
add_subdirectory (Crow)
add_subdirectory (fmod)
add_subdirectory (fsbank)
add_subdirectory (glaze)
add_subdirectory (jwt-cpp)
add_subdirectory (mongo-cxx-driver)
set_target_properties(mongocxx_shared PROPERTIES
    MONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX ON
)

add_subdirectory (spdlog)
set_target_properties(spdlog PROPERTIES
    SPDLOG_BUILD_SHARED OFF
    BUILD_SHARED_LIBS OFF
)

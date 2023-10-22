add_subdirectory (fmod)
add_subdirectory (fsbank)
add_subdirectory (bcrypt)
add_subdirectory (Crow)
add_subdirectory (Catch2)
add_subdirectory (jwt-cpp)

set (CMAKE_ENABLE_EXPORTS ON)
add_subdirectory (curl)

add_subdirectory (glaze)
add_subdirectory (mongo-cxx-driver)

add_subdirectory (spdlog)
set_target_properties(spdlog PROPERTIES
    SPDLOG_BUILD_SHARED OFF
)

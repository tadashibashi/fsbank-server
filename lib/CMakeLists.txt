add_subdirectory (fmod)
add_subdirectory (fsbank)
add_subdirectory (poco)
add_subdirectory (Catch2)
add_subdirectory (glaze)

set (MONGOCXX_BUILD_STATIC 1)
add_subdirectory (mongo-cxx-driver)

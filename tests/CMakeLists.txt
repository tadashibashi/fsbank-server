project (insound-tests)

file (GLOB_RECURSE INSOUND_TEST_SRC ./*.test.cpp)
add_executable (${PROJECT_NAME} ${INSOUND_TEST_SRC})

target_link_libraries (${PROJECT_NAME} PRIVATE insound Catch2::Catch2WithMain)

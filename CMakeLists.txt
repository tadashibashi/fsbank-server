cmake_minimum_required (VERSION 3.14)
project(insound)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Set module path
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

# Global cmake configurations
set(CMAKE_CXX_STANDARD 20)
include(ConfigMessages)
include(FormatBuildType)
enable_testing()

# Include projects
add_subdirectory(lib)
add_subdirectory(insound)

# Format cmake build type to title-case
# Only permits "Release", "Debug", "RelWithDebInfo", & "MinSizeRel"
# but with any casing where e.g. "rElEAse" is okay.
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER)
if ( ${BUILD_TYPE_UPPER} MATCHES "DEBUG" )
    set(CMAKE_BUILD_TYPE "Debug")
elseif ( ${BUILD_TYPE_UPPER} MATCHES "RELEASE" )
    set(CMAKE_BUILD_TYPE "Release")
elseif ( ${BUILD_TYPE_UPPER} MATCHES "RELWITHDEBINFO" )
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
elseif ( ${BUILD_TYPE_UPPER} MATCHES "MINSIZEREL" )
    set(CMAKE_BUILD_TYPE "MinSizeRel")
else()
    message( FATAL_ERROR "Build type ${CMAKE_BUILD_TYPE} not recognized." )
endif()

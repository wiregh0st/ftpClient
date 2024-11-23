# CMakeList.txt : CMake project for ftpRetry, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(PROJECT_NAME "ftpRetry")
project (${PROJECT_NAME})

set(CMAKE_BUILD_TYPE Debug)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)

# Add source to this project's executable.
add_executable (ftpRetry ${SRC_FILES})

if (WIN32)
   target_link_libraries(ftpRetry comctl32)
endif()

set_target_properties(ftpRetry PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")


if (CMAKE_VERSION VERSION_GREATER 3.12)
 set_property(TARGET ftpRetry PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
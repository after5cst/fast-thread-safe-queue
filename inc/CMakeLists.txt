# CMakeLists files in this project can
# refer to the root source directory of the project as ${FTSQ_SOURCE_DIR} and
# to the root binary directory of the project as ${FTSQ_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (FTSQ)

# Set the output executables to go to a single directory on the
# build tree.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Automatically include this directory in all subprojects
include_directories(inc)

#And now go into the test directory to build test apps.
add_subdirectory (test)


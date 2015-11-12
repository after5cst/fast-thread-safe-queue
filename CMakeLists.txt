cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
# CMakeLists files in this project can
# refer to the root source directory of the project as ${FTSQ_SOURCE_DIR} and
# to the root binary directory of the project as ${FTSQ_BINARY_DIR}.
project (FTSQ)

include_directories("inc")

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (test)


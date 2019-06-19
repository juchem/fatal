cmake_minimum_required (VERSION 3.12)

project (fatal)

set(fatal_VERSION_MAJOR 1)
set(fatal_VERSION_MINOR 0)

#file(
#  GLOB_RECURSE
#    LIBFATAL_SOURCE_HEADERS
#    CONFIGURE_DEPENDS
#    LIST_DIRECTORIES false
#    "*.h"
#)
#message(STATUS "libfatal headers: ${LIBFATAL_SOURCE_HEADERS}")

#file(
#  GLOB_RECURSE
#    LIBFATAL_SOURCE_TESTS
#    CONFIGURE_DEPENDS
#    LIST_DIRECTORIES false
#    "*_test.cpp"
#)
#message(STATUS "libfatal tests: ${LIBFATAL_SOURCE_TESTS}")

#file(
#  GLOB_RECURSE
#    LIBFATAL_SOURCE_BENCHMARKS
#    CONFIGURE_DEPENDS
#    LIST_DIRECTORIES false
#    "*_benchmark.cpp"
#)
#message(STATUS "libfatal benchmarks: ${LIBFATAL_SOURCE_BENCHMARKS}")

add_library(libfatal INTERFACE)

target_compile_features(
  libfatal
  INTERFACE
    cxx_std_17
)

target_compile_options(
  libfatal
  INTERFACE
    -Wno-gnu-string-literal-operator-template
)

target_include_directories(
  libfatal
  INTERFACE
    $<INSTALL_INTERFACE:include>
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/fatal"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  FILES_MATCHING PATTERN "*.h"
)

install(
  TARGETS libfatal
  DESTINATION "${CMAKE_INSTALL_PREFIX}"
  EXPORT libfatal-config
)

install(
	EXPORT libfatal-config
  DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

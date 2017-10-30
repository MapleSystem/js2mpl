cmake_minimum_required(VERSION 3.0)

# Put all built library files under lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Select flags.
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -std=c++11 ")

if( ${uppercase_CMAKE_BUILD_TYPE} STREQUAL "DEBUG" )
  add_definitions(-DMIR_DEBUG=1)
endif()
if( ${uppercase_CMAKE_BUILD_TYPE} STREQUAL "RELWITHDEBINFO" )
  add_definitions(-DDEBUG=0)
endif()

SET(MOZJS_ROOT ${CMAKE_SOURCE_DIR}/mozjs)
SET(MOZJS_SOURCE_DIR ${MOZJS_ROOT}/mozjs-31.2.0)
SET(MOZJS_BUILD_DIR ${MOZJS_ROOT}/mozjs-build)
SET(MOZJS_INSTALL_DIR ${MOZJS_BUILD_DIR}/dist)

add_subdirectory(src)

cmake_minimum_required(VERSION 3.0)

#### tools used for compilation ####
message(STATUS "Use GNU compiler")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to RelWithDebInfo")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Build type (default RelWithDebInfo)")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE AND
    NOT uppercase_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE|RELWITHDEBINFO)$")
  message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

# The main project
project(js2mpl)

enable_language(C)
enable_language(CXX)

# Dump the building details
#set(CMAKE_VERBOSE_MAKEFILE on)

# Put all built library files under lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Put all built binary files under bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT  HOST_ARCH)
 SET(HOST_ARCH "64")
endif(NOT HOST_ARCH)

if( ${HOST_ARCH} EQUAL 64 )
  SET(HOST_MACH "-m64")
else ( ${HOST_ARCH} EQUAL 64 )
  SET(HOST_MACH "-m32")
endif( ${HOST_ARCH} EQUAL 64 )

# Select flags.
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 ${HOST_MACH} -w")

# CMAKE_CXX_FLAGS_DEBUG, CMAKE_CXX_FLAGS_RELEASE, CMAKE_CXX_FLAGS_RELWITHDEBINFO are
# appended to CMAKE_CXX_FLAGS
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g3")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g3")

if( ${uppercase_CMAKE_BUILD_TYPE} STREQUAL "DEBUG" )
  add_definitions(-DMIR_DEBUG=1)
endif()
#if( ${uppercase_CMAKE_BUILD_TYPE} STREQUAL "RELWITHDEBINFO" )
#  add_definitions(-DDEBUG=0)
#endif()

option(DYNAMICLANG "Enable the support for dynamic types" ON)
if(DYNAMICLANG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDYNAMICLANG")
endif()

option(RC_V2 "RC_V2" ON)
if(RC_V2)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRC_NO_MMAP")
endif()

option(TEST_BENCHMARK "Test benchmark" OFF)
if(TEST_BENCHMARK)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTEST_BENCHMARK")
endif()

option(MEMORY_LEAK_CHECK "Check memory leaks" OFF)
if(MEMORY_LEAK_CHECK)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMEMORY_LEAK_CHECK")
endif()

option(MARK_CYCLE_ROOTS "Mark cycle roots" OFF)
if(MARK_CYCLE_ROOTS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMARK_CYCLE_ROOTS")
endif()

option(DEX "Set DEX as the target" ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMIR_DEX=0")

SET(MOZJS_ROOT ${CMAKE_SOURCE_DIR}/../mozjs)
SET(MOZJS_SOURCE_DIR ${MOZJS_ROOT}/mozjs-31.2.0)
SET(MOZJS_BUILD_DIR ${MOZJS_ROOT}/mozjs-build)
SET(MOZJS_INSTALL_DIR ${MOZJS_BUILD_DIR}/dist)

SET(STATIC_LIBMAPLEIR ${CMAKE_SOURCE_DIR}/../mapleall/build/gnu/mapleir/libmpl.a) 
SET(STATIC_LIBMEMPOOL ${CMAKE_SOURCE_DIR}/../mapleall/build/gnu/Mempool/libmempool.a)

add_subdirectory(src)

#
# ============================================================================
# John Ratz PROPRIETARY
# ============================================================================
# This file and its contents are the intellectual property of John Ratz.  It may
# not be used, copied, distributed or otherwise  disclosed in whole or in part
# without the express written permission of John Ratz.
# ============================================================================
# Copyright (c) 2021, John Ratz. All rights reserved.
# ============================================================================
#

# This file sets up External projects used in this contrib directory
include(ExternalProject)
include(FindPkgConfig)

###########################################################
# FlatBuffers
###########################################################

option(BUILD_FLATBUFFERS "Build google flatbuffers" ON)

if(BUILD_FLATBUFFERS)
    ExternalProject_add(FlatBuffers
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers
            BINARY_DIR ${PROJECT_BINARY_DIR}/flatbuffers
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT} -DFLATBUFFERS_BUILD_TESTS:BOOL=OFF -DFLATBUFFERS_BUILD_FLATHASH:BOOL=OFF -DFLATBUFFERS_BUILD_FLATLIB:BOOL=OFF
            EXCLUDE_FROM_ALL 1
            )
    add_library(flatbuffers_api INTERFACE)
    add_dependencies(flatbuffers_api FlatBuffers)
    target_include_directories(flatbuffers_api INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include)
    set(FLATC_BINARY "${PROJECT_BINARY_DIR}/flatbuffers/flatc" PARENT_SCOPE)
endif()

###########################################################
# spdlog
###########################################################

option(BUILD_SPDLOG "Build spdlog" ON)

if(BUILD_SPDLOG)
    ExternalProject_add(spdlog
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog
            BINARY_DIR ${PROJECT_BINARY_DIR}/spdlog
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_BUILD_TESTS=OFF -DSPDLOG_BUILD_BENCH=OFF
            EXCLUDE_FROM_ALL 1
            )
    add_library(spdlog_api INTERFACE)
    add_dependencies(spdlog_api spdlog)
    target_include_directories(spdlog_api INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include)
endif()

###########################################################
# libzmq
###########################################################

configure_file("libzmq.CMakeLists.txt.in" "libzmq-download/CMakeLists.txt")
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/contrib/libzmq-download)

if (result)
    message(FATAL_ERROR "CMake step for libzmq failed: ${result}")
else()
    message(STATUS "CMake passed for libzmq")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/contrib/libzmq-download)
if (result)
    message(FATAL_ERROR "Build step for libzmq failed: ${result}")
else()
    message(STATUS "Successfully built libzmq")
endif()

###########################################################
# cppzmq
###########################################################
option(BUILD_CPPZMQ "Build cppzmq" ON)
if (BUILD_CPPZMQ)
    ExternalProject_add(cppzmq_project
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cppzmq
            BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/cppzmq
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT} -DCPPZMQ_BUILD_TESTS=OFF)
    add_library(cppzmq INTERFACE)
    add_dependencies(cppzmq libzmq cppzmq_project)
    target_include_directories(cppzmq INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include)
endif()

###########################################################
# yaml-cpp
###########################################################
option(BUILD_YAML_CPP "Build yaml-cpp" ON)
if (BUILD_YAML_CPP)
    ExternalProject_add(yaml_cpp_project
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp
            BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp
            CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT} -DYAML_BUILD_SHARED_LIBS:BOOL=OFF -DYAML_CPP_BUILD_TESTS:BOOL=OFF -DYAML_CPP_BUILD_TOOLS:BOOL=OFF -DYAML_CPP_BUILD_CONTRIB:BOOL=OFF)
    add_library(yaml-cpp INTERFACE)

    add_dependencies(yaml-cpp yaml_cpp_project)
    target_link_libraries(yaml-cpp INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/lib/libyaml-cpp.a)
    target_include_directories(yaml-cpp INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include/yaml-cpp)
endif()

############################################################
## linenoise-ng  (command line entry reader/parser)
## See: https://github.com/arangodb/linenoise-ng
## snapshot of repo is at contrib/linenoise-ng
############################################################
#option(BUILD_LINENOISE_NG "Build linenoise-ng" ON)
#
#if(BUILD_LINENOISE_NG)
#	ExternalProject_add(linenoise
#		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/linenoise-ng
#		BINARY_DIR ${PROJECT_BINARY_DIR}/linenoise-ng/
#		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#		EXCLUDE_FROM_ALL 1
#		)
#	add_library(linenoise_api INTERFACE)
#	add_dependencies(linenoise_api linenoise)
#	target_include_directories(linenoise_api INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include)
#	target_link_libraries(linenoise_api INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/lib/liblinenoise.a)
#endif()
#
############################################################
## pugixml
############################################################
#option(BUILD_PUGIXML "Build pugixml" ON)
#
#if(BUILD_PUGIXML)
#	set(PUGIXML_CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT} -DBUILD_PKGCONFIG:BOOL=ON -DCMAKE_CXX_FLAGS=-fPIC)
#
#	ExternalProject_add(libpugixml
#		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pugixml
#		CMAKE_ARGS ${PUGIXML_CMAKE_ARGS}
#		EXCLUDE_FROM_ALL 1
#		)
#
#	add_library(pugixml INTERFACE)
#	add_dependencies(pugixml libpugixml)
#	target_link_libraries(pugixml INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/lib/libpugixml.a)
#	target_include_directories(pugixml INTERFACE ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include)
#endif()

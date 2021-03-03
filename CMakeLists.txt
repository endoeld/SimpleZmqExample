cmake_minimum_required(VERSION 3.15)

if ("$ENV{BUILD_NUMBER}" STREQUAL "")
    set(TWEAK_NUM 0)
else()
    set(TWEAK_NUM $ENV{BUILD_NUMBER})
endif()

# Define the project
project(SimpleZmqExample 1.0.0.${TWEAK_NUM})

add_definitions(-DVERSION_MAJOR=${PROJECT_VERSION_MAJOR} -DVERSION_MINOR=${PROJECT_VERSION_MINOR} -DVERSION_PATCH=${PROJECT_VERSION_PATCH} -DBUILD_NUMBER=${TWEAK_NUM})

# Setup the GNU install directories
include(GNUInstallDirs)
mark_as_advanced(CLEAR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)

set(SIMPLE_ZMQ_EXAMPLE_SYS_ROOT "${PROJECT_BINARY_DIR}/sysroot")
file(MAKE_DIRECTORY ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include)

# JMR ToDo 20210302 - Figure out if this actually needed on MacOS
if (APPLE)
    add_compile_options(-ferror-limit=0)
endif (APPLE)

# Add the sysroot/include folder
include_directories(${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT}/include)

###############################################################################
# Build options
###############################################################################

# Whether or not to build tests
option(BUILD_TESTS "Build Unit Tests" OFF)

# JMR ToDo 20210302 - Add basic testing
if(BUILD_TESTS)
    # If tests are being built, set flag for it
    add_definitions(-DTEST_BUILD)
endif()

# Following options are used to turn on Wall, Werror, and Wextra, respectively
# The "add_compile_options" function adds the flags to all flags

option(USE_WALL "Use Wall flag" ON)
if (USE_WALL)
    add_compile_options(-Wall)
endif()

option(USE_WERROR "Use Werror flag" ON)
if (USE_WERROR)
    add_compile_options(-Werror)
endif()

option(USE_WEXTRA "Use Wextra flag" ON)
if (USE_WEXTRA)
    add_compile_options(-Wextra)
endif()

# Set the C++ dialect to -std=c++17
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    if (APPLE)
        message(FATAL_ERROR "Unfortunately, no coverage for you!  Once you figure out how to generate coverage on a MacOS using Clang, put it in here!")
    endif()
    add_definitions(-DCOVERAGE_ENABLED)
    add_compile_options(-g -O0 --coverage -fprofile-arcs -ftest-coverage)
    link_libraries(gcov)
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/coverage/index.html
            COMMAND lcov -t "coverage" -o coverage.info -c -d ${PROJECT_BINARY_DIR}/src
            COMMAND lcov -r coverage.info "/opt/rh/devtoolset-9/root/usr/include/*" "/usr/include/*" "*googletest-src*" "gtest*" "*googletest*" "*/include/spdlog/*" "*ApplianceUtilities/include/*" "*MicroServiceError*" "*flatbuffers*" "*include/zmq.hpp"  -o coverage-filtered.info
            COMMAND genhtml -o coverage coverage-filtered.info
            COMMENT "Generating Coverage HTML files"
            VERBATIM
            )
    add_custom_target(coverage DEPENDS ${CMAKE_BINARY_DIR}/coverage/index.html)
endif()

set(CMAKE_PREFIX_PATH ${SIMPLE_ZMQ_EXAMPLE_SYS_ROOT})

add_subdirectory(${PROJECT_SOURCE_DIR}/contrib)
# JMR ToDo 20210302 - Create docs dir and add initial documentation
#add_subdirectory(${PROJECT_SOURCE_DIR}/docs)

# Define addresses for service communication used in configuration file
set(APPLIANCE_CONTROL_COMMAND_ADDRESS \"tcp://127.0.0.1:7770\")
add_definitions(-DAPPLIANCE_CONTROL_COMMAND_ADDRESS=${APPLIANCE_CONTROL_COMMAND_ADDRESS})
set(APPLIANCE_CONTROL_HEARTBEAT_ADDRESS \"tcp://127.0.0.1:7771\")
add_definitions(-DAPPLIANCE_CONTROL_HEARTBEAT_ADDRESS=${APPLIANCE_CONTROL_HEARTBEAT_ADDRESS})

set(APPLIANCE_LOG_COMMAND_ADDRESS \"tcp://127.0.0.1:7772\")
add_definitions(-DAPPLIANCE_LOG_COMMAND_ADDRESS=${APPLIANCE_LOG_COMMAND_ADDRESS})
set(APPLIANCE_LOG_HEARTBEAT_ADDRESS \"tcp://127.0.0.1:7773\")
add_definitions(-DAPPLIANCE_LOG_HEARTBEAT_ADDRESS=${APPLIANCE_LOG_HEARTBEAT_ADDRESS})

# Default log level to use for spdlog loggers in the system
set(DEFAULT_SPDLOG_LOG_LEVEL info)
add_definitions(-DDEFAULT_SPDLOG_LOG_LEVEL=${DEFAULT_SPDLOG_LOG_LEVEL})

# Name to use for log file (syslog)
set(APPLIANCE_LOG_FILE_NAME qflex_appliance.log)
add_definitions(-DAPPLIANCE_LOG_FILE_NAME=${APPLIANCE_LOG_FILE_NAME})

# Maximum size for log files set to 2 GB (syslog)
set(APPLIANCE_MAX_LOG_SIZE_BYTES 2147482548)
add_definitions(-DAPPLIANCE_MAX_LOG_SIZE_BYTES=${APPLIANCE_MAX_LOG_SIZE_BYTES})

# Minimum size for log files set to 1 MB (syslog)
set(APPLIANCE_MIN_LOG_SIZE_BYTES 1048576)
add_definitions(-DAPPLIANCE_MIN_LOG_SIZE_BYTES=${APPLIANCE_MIN_LOG_SIZE_BYTES})

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/linux_distribution)

add_subdirectory(${PROJECT_SOURCE_DIR}/packages)

if (BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
endif (BUILD_TESTS)

set(UNCRUSTIFY_CONFIG_FILE ${PROJECT_SOURCE_DIR}/data/hsm_uncrustify.cfg)

add_custom_target(indent
        find ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/test -type f \( -iname "*.c" -o -iname "*.cpp" -o -iname "*.h" -o -iname "*.hpp" \) -exec uncrustify -c ${UNCRUSTIFY_CONFIG_FILE} --no-backup {} \;
        COMMENT "Running Uncrustify"
        VERBATIM
        )

include(FindPkgConfig)
pkg_check_modules(ZMQ REQUIRED IMPORTED_TARGET libzmq)

# Define root path for installation of SimpleZmqExample files
set(SIMPLE_ZMQ_EXAMPLE_BASE_INSTALL_PATH opt/simple_zmq_example)
add_definitions(-DSIMPLE_ZMQ_EXAMPLE_BASE_INSTALL_PATH=${SIMPLE_ZMQ_EXAMPLE_BASE_INSTALL_PATH})

add_executable(SimpleZmqExample SimpleHelloWorld.cpp)

add_executable(HelloWorldServer HelloWorldServer.cpp)
target_link_libraries(HelloWorldServer zmq)

add_executable(HelloWorldClient HelloWorldClient.cpp)
target_link_libraries(HelloWorldClient zmq)

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

add_executable(HelloWorldClient
        ${CMAKE_CURRENT_SOURCE_DIR}/HelloWorldClient.cpp)

target_link_libraries(HelloWorldClient fmt spdlog_api yaml-cpp)

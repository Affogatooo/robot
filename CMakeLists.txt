cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_FLAGS "-std=gnu++23")
set(EXTRA_COMPONENT_DIRS core)
set(COMPONENTS main core)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(Robot)
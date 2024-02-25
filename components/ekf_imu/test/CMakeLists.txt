cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(COMPONENTS main)

idf_build_set_property(COMPILE_DEFINITIONS "NO_DEBUG_STORAGE" APPEND)
project(test_mat_utils)


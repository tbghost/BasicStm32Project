# ================================================================================
# ROOT CMake Listfile
# This file must be stay at the root-directory of a project
# CMake uses the root directory for keyword ${CMAKE_CURRENT_SOURCE_DIR}
# call cmake: cmake -DCMAKE_BUILD_TYPE="Debug" -DPLATFORM="Unittest" ..
# ================================================================================

# minimum requirement
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)


# set Project properties
set(PROJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(PRJNAME BasicProject)
set(MAJOR 1)
set(MINOR 0)
set(PATCH 0)
set(PRJVERSION ${MAJOR}.${MINOR}.${PATCH})
project(${PRJNAME} VERSION ${PRJVERSION})

message (" ")
message ("-- Entering ${PROJ_PATH}/CMakeLists.txt")
message ("-- ProjectName: ${PRJNAME}")
message ("-- ProjectVersion (Major.Minor.Patch): ${PRJVERSION}")
message ("-- Build type: ${CMAKE_BUILD_TYPE}")
message ("-- Platform: ${PLATFORM}")
message (" ")

enable_language(C CXX ASM)

# C und C++ Standard (optional, falls vom Projekt benötigt)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(${PLATFORM} STREQUAL "Baremetal")
    include(cmake/CMakeLists.Baremetal.cmake)
elseif(${PLATFORM} STREQUAL "Unittest")
    include(cmake/CMakeLists.Unittest.cmake)
else()
    message(FATAL_ERROR "Error: Build PLATFORM invalid.")
endif()


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
set(PRJNAME BasicStm32Project)
set(MAJOR 1)
set(MINOR 0)
set(PATCH 0)
set(PRJVERSION ${MAJOR}.${MINOR}.${PATCH})
project(${PRJNAME} VERSION ${PRJVERSION})


if(NOT DEFINED SW_ID)
    set(SW_ID 10000)
endif()

if(NOT DEFINED HW_ID)
    set(HW_ID 10000)
endif()

# add commit info git
function(check_command cmd result_var)
    execute_process(
        COMMAND ${cmd} --version
        RESULT_VARIABLE _git_check_vers
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if (_git_check_vers EQUAL 0)
        set(${result_var} TRUE PARENT_SCOPE)
    else()
        set(${result_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

check_command(git HAS_GIT)
if(HAS_GIT)
    message (" ")
    message("------------ Git Identification --------------")
    #branch 
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE GIT_COMMIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("BRANCH: ${GIT_COMMIT_BRANCH}")          

    #first 8 nibbles of git-hash 
    execute_process(
        COMMAND git rev-parse --short=8 HEAD
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("HASH: ${GIT_COMMIT_HASH}")        
    
    #commit message 
    execute_process(
        COMMAND git log -1 --pretty=oneline --abbrev-commit
        OUTPUT_VARIABLE GIT_COMMIT_MSG
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("MSG: ${GIT_COMMIT_MSG}")   
    message("----------------------------------------------")     
   
    set(GIT_BRANCH ${GIT_COMMIT_BRANCH} CACHE INTERNAL "Git current branch")
    set(GIT_HASH   ${GIT_COMMIT_HASH} CACHE INTERNAL "Git commit hash")
    set(GIT_MSG    ${GIT_COMMIT_MSG} CACHE INTERNAL "Git commit message")
else()
    # dummy commit info
    set(GIT_BRANCH "Unknown" CACHE INTERNAL "Git current branch")
    set(GIT_HASH   "Unknown" CACHE INTERNAL "Git commit hash")
    set(GIT_MSG    "Unknown" CACHE INTERNAL "Git commit message")
endif()

# add cmake variables for using in source code
add_definitions(-DGIT_BRANCH="${GIT_BRANCH}" )
add_definitions(-DGIT_HASH="${GIT_HASH}" )
add_definitions(-DGIT_MSGBRANCH="${GIT_MSG}" )
add_definitions(-DSW_RELEASE="${PRJVERSION}" )
add_definitions(-DSW_ID="${SW_ID}" )
add_definitions(-DHW_ID="${HW_ID}" )

message (" ")
message ("------------ Project Identification ----------")     
message ("-- Entering ${PROJ_PATH}/CMakeLists.txt")
message ("-- ProjectName: ${PRJNAME}")
message ("-- ProjectVersion (Major.Minor.Patch): ${PRJVERSION}")
message ("-- Hardware ID: ${HW_ID}")
message ("-- Software ID: ${SW_ID}")
message ("-- Build type: ${CMAKE_BUILD_TYPE}")
message ("-- Platform: ${PLATFORM}")
message ("-- Toolchain: ${TOOLCHAIN_PREFIX}")
message ("----------------------------------------------")     

enable_language(C CXX ASM)

# C und C++ Standard (optional, falls vom Projekt benötigt)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


if(${PLATFORM} STREQUAL "Baremetal")
    
    # add threadx os library
    add_library(threadx STATIC)
    target_sources(threadx PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/common/src/tx_initialize_kernel_enter.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/common/src/tx_thread_create.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/common/src/tx_thread_resume.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/common/src/tx_thread_sleep.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/common/src/tx_mutex_create.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/common/src/tx_semaphore_create.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os/startup/tx_initialize_low_level.S
        )
    target_include_directories(threadx PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/common/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/os/threadx/ports/cortex_m7/gnu/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/os/config
    )
    
    include(cmake/CMakeLists.Baremetal.cmake)

elseif(${PLATFORM} STREQUAL "Unittest")
    include(cmake/CMakeLists.Unittest.cmake)
else()
    message(FATAL_ERROR "Error: Build PLATFORM invalid.")
endif()


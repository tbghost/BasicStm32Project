# ================================================================================
# ROOT/cmake/CMakeLists.Unittest
# This CMake include file called by the ROOT/CMakeLists.txt
# ===============================================================================

    
### platform unittest ###
set(EXECUTABLE ${PROJECT_NAME}.elf)

message (" ")
message ("-- Toolchain: ${TOOLCHAIN_PREFIX}")
message ("-- Device: Native")
message (" ")

################################################################################
# Global Sources
################################################################################
set(SOURCES 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)
################################################################################
# Global Includes
################################################################################
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
)
################################################################################
# Add the subdirectories which includes used libs with own CmakeLists.txt
################################################################################
add_subdirectory(src/utils)
add_subdirectory(lib/googletest)
add_subdirectory(tests) 

# add executable 
add_executable(${EXECUTABLE} ${SOURCES})

# link executable with library
target_link_libraries(${EXECUTABLE} 
        PRIVATE 
        Utils)

### end of unittest ###

# ================================================================================
# ROOT/cmake/CMakeLists.Baremetal
# This CMake include file called by the ROOT/CMakeLists.txt
# ===============================================================================

### platform baremetal ###

set(EXECUTABLE ${PROJECT_NAME}.elf)

################################################################################
# Global Flags and definitions
################################################################################
math(EXPR FLASH_START "0x08000000" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR FLASH_LENGTH "0x200000" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR FLASH_END "${FLASH_START} + ${FLASH_LENGTH} - 1" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR DATA_RAM_START "0x20000000" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR DATA_RAM_LENGTH "0x20000" OUTPUT_FORMAT HEXADECIMAL)


add_compile_definitions(FLASH_START=${FLASH_START})
add_compile_definitions(FLASH_LENGTH=${FLASH_LENGTH})

message (" ")
message ("----------Device Identification---------------")     

message ("-- Device:        ${CHIP}")
message ("-- FLASH_START:   ${FLASH_START}")
message ("-- FLASH_LENGTH:  ${FLASH_LENGTH}")
message ("----------------------------------------------")     

################################################################################
# Global Sources
################################################################################
set(SOURCES 
    ${CMAKE_SOURCE_DIR}/hal/main.cpp
    ${CMAKE_SOURCE_DIR}/os/posix/posix.cpp
    ${CMAKE_SOURCE_DIR}/os/posix/time.cpp
    #${CMAKE_SOURCE_DIR}/hal/startup/startup_stm32h743zitx.s
    )
################################################################################
# Global Includes
################################################################################
include_directories(
    ${CMAKE_SOURCE_DIR}/os/threadx/common/inc
    ${CMAKE_SOURCE_DIR}/os/threadx/ports/cortex_m7/gnu/inc
    ${CMAKE_SOURCE_DIR}/os/posix
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/hal
    ${CMAKE_SOURCE_DIR}/hal/cmsis
    ${CMAKE_SOURCE_DIR}/hal/hal_driver
    ${CMAKE_SOURCE_DIR}/hal/startup
    ${CMAKE_SOURCE_DIR}/os/config
    )
################################################################################
# Add the subdirectories which includes used libs with own CmakeLists.txt
################################################################################
add_subdirectory(src/utils)
add_subdirectory(hal)


# add executable 
add_executable(${EXECUTABLE} ${SOURCES})

#target_include_directories(${EXECUTABLE}  
#            PUBLIC 
#            "./platform/src_notest/boards/nucleo_h743zi2/"
#            )

#libraries
target_link_libraries(${EXECUTABLE} PRIVATE
          Utils
          HAL          
          threadx
          )

set_target_properties(${EXECUTABLE} PROPERTIES
        LINKER_LANGUAGE CXX
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
        OUTPUT_NAME "${PRJNAME}"
        SUFFIX ".elf"
       )        
        
                
# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>)
# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex)
# Conver to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin)   
    
### end of baremetal target ###


# ================================================================================
# ROOT/cmake/CMakeLists.Baremetal
# This CMake include file called by the ROOT/CMakeLists.txt
# ===============================================================================

### platform baremetal ###

set(EXECUTABLE ${PROJECT_NAME}.elf)

################################################################################
# FLASH 
################################################################################
math(EXPR FLASH_START "0x08000000" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR FLASH_SIZE  "0x200000" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR FLASH_END "${FLASH_START} + ${FLASH_SIZE} - 1" OUTPUT_FORMAT HEXADECIMAL)

## Interrupt vector table ##
math(EXPR FLASHVTABLE_START "${FLASH_START}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR FLASHVTABLE_SIZE "0x300" OUTPUT_FORMAT HEXADECIMAL)       

## Id + checksum ##
math(EXPR CHECKSUM_SIZE "0x4" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR FLASHIDAREA_SIZE "0x100-${CHECKSUM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR FLASHIDAREA_START "${FLASHVTABLE_START} + ${FLASHVTABLE_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CHECKSUM_START "${FLASHIDAREA_START} + ${FLASHIDAREA_SIZE} - ${CHECKSUM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)

## Flash Application ##
math(EXPR APP_SIZE "${FLASH_SIZE} - ${FLASHVTABLE_SIZE} - ${FLASHIDAREA_SIZE} - ${CHECKSUM_SIZE} " OUTPUT_FORMAT HEXADECIMAL)            
math(EXPR APP_START "${FLASH_START} + ${FLASHVTABLE_SIZE} + ${FLASHIDAREA_SIZE} + ${CHECKSUM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)

#add_compile_definitions(FLASH_START=${FLASH_START})
#add_compile_definitions(FLASH_LENGTH=${FLASH_LENGTH})

message (" ")
message ("---------- Environment ---------------")     
message ("-- Device:            ${CHIP}")
message ("-- FLASH_START:       ${FLASH_START}")
message ("-- FLASH_SIZE:        ${FLASH_SIZE}")
message ("-- FLASH_END:         ${FLASH_END}")
message ("-- FLASHVTABLE_START: ${FLASHVTABLE_START}")
message ("-- FLASHVTABLE_SIZE:  ${FLASHVTABLE_SIZE}")
message ("-- FLASHIDAREA_START: ${FLASHIDAREA_START}")
message ("-- FLASHIDAREA_SIZE:  ${FLASHIDAREA_SIZE}")
message ("-- CHECKSUM_START:    ${CHECKSUM_START}")
message ("-- CHECKSUM_SIZE:     ${CHECKSUM_SIZE}")
message ("-- APP_START:         ${APP_START}")
message ("-- APP_SIZE:          ${APP_SIZE}")


################################################################################
# RAM 
################################################################################

math(EXPR DTCMRAM_SIZE    "0x20000" OUTPUT_FORMAT HEXADECIMAL)     # 1310720 B DTCM = Data Tightly-coupled mem
math(EXPR ITCMRAM_SIZE    "0x10000" OUTPUT_FORMAT HEXADECIMAL)     # 65536 B ITCM = Instruction Tightly-coupled mem        
math(EXPR RAM_D1_SIZE     "0x80000" OUTPUT_FORMAT HEXADECIMAL)     # 524288 B 
math(EXPR RAM_D2_SIZE     "0x48000" OUTPUT_FORMAT HEXADECIMAL)     # 294912 B 
math(EXPR RAM_D3_SIZE     "0x10000" OUTPUT_FORMAT HEXADECIMAL)     # 65536 B 
math(EXPR DTCMRAM_START   "0x20000000" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR ITCMRAM_START   "0x00000000" OUTPUT_FORMAT HEXADECIMAL)  
math(EXPR RAM_D1_START    "0x24000000" OUTPUT_FORMAT HEXADECIMAL)             
math(EXPR RAM_D2_START    "0x30000000" OUTPUT_FORMAT HEXADECIMAL)             
math(EXPR RAM_D3_START    "0x38000000" OUTPUT_FORMAT HEXADECIMAL)             

## RAM Interrrupt vector table ##
math(EXPR RAMVTABLE_START "${RAM_D1_START}" OUTPUT_FORMAT HEXADECIMAL) # addr 0x24000000 
math(EXPR RAMVTABLE_SIZE  "0x300" OUTPUT_FORMAT HEXADECIMAL)        # 768 B 

## NOINIT Area ##
math(EXPR NOINIT_SIZE            "0x100" OUTPUT_FORMAT HEXADECIMAL)                  # 256 B 
math(EXPR NOINIT_START           "${RAMVTABLE_START} + ${RAMVTABLE_SIZE}" OUTPUT_FORMAT HEXADECIMAL) # addr 0x24000300    

## RAM Stack ##
math(EXPR STACKSIZE       "0x800" OUTPUT_FORMAT HEXADECIMAL)                 # 2048 B      
math(EXPR STACKSTART      "${RAM_D1_START} + ${RAM_D1_SIZE} - ${STACKSIZE}" OUTPUT_FORMAT HEXADECIMAL)   

math(EXPR RAM_D1_APPSIZE         "${RAM_D1_SIZE} - ${RAMVTABLE_SIZE} - ${NOINIT_SIZE} - ${STACKSIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR RAM_D1_APPSTART        "${NOINIT_START} + ${NOINIT_SIZE}" OUTPUT_FORMAT HEXADECIMAL)

#math(EXPR DATA_RAM_START "0x20000000" OUTPUT_FORMAT HEXADECIMAL)
#math(EXPR DATA_RAM_LENGTH "0x20000" OUTPUT_FORMAT HEXADECIMAL)
message ("----------------------------------------------")     
message ("-- DTCMRAM_START:       ${DTCMRAM_START}")
message ("-- DTCMRAM_SIZE:        ${DTCMRAM_SIZE}")
message ("-- ITCMRAM_START:       ${ITCMRAM_START}")
message ("-- ITCMRAM_SIZE:        ${ITCMRAM_SIZE}")
message ("-- RAM_D1_START:        ${RAM_D1_START}")
message ("-- RAM_D1_SIZE:         ${RAM_D1_SIZE}")
message ("-- RAM_D2_START:        ${RAM_D2_START}")
message ("-- RAM_D2_SIZE:         ${RAM_D2_SIZE}")  
message ("-- RAM_D3_START:        ${RAM_D3_START}")
message ("-- RAMVTABLE_START:     ${RAMVTABLE_START}")
message ("-- RAMVTABLE_SIZE:      ${RAMVTABLE_SIZE}")
message ("-- NOINIT_START:         ${NOINIT_START}")
message ("-- NOINIT_SIZE:          ${NOINIT_SIZE}")
message ("-- STACKSTART:          ${STACKSTART}")
message ("-- STACKSIZE:           ${STACKSIZE}")
message ("-- RAM_D1_APPSTART:     ${RAM_D1_APPSTART}")
message ("-- RAM_D1_APPSIZE:      ${RAM_D1_APPSIZE}")


################################################################################
# Global Sources
################################################################################
set(SOURCES 
    ${CMAKE_SOURCE_DIR}/hal/main.cpp
    ${CMAKE_SOURCE_DIR}/os/posix/posix.cpp
    ${CMAKE_SOURCE_DIR}/os/posix/time.cpp
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
#          threadx
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


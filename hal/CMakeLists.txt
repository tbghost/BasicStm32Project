# ================================================================================
# CMake Listfile root/hal
# ================================================================================

# STM32 HAL Drivers library
set(HAL_DRIVER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_driver/Src/stm32h7xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_driver/Src/stm32h7xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal_driver/Src/stm32h7xx_ll_utils.c
    )

set(SYSTEM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/system/stm32h7xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system/system_stm32h7xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system/hal_init.c
    
    )

set(CMSIS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Device/ST/STM32H7xx/Source/Templates/system_stm32h7xx.c
    )

set(STARTUP_SRC
    #${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Device/ST/STM32H7xx/Source/Templates/arm/startup_stm32h743xx.s
    ${CMAKE_CURRENT_SOURCE_DIR}/startup/startup_stm32h743zitx.s
    )


# add components as library
add_library(HAL
            STATIC
            ${HAL_DRIVER_SRC}
            ${SYSTEM_SRC}
            ${CMSIS_SRC}
            ${STARTUP_SRC}
            )

# Include directories für die HAL-Bibliothek
target_include_directories(HAL
            PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/hal_driver/Inc
            ${CMAKE_CURRENT_SOURCE_DIR}/hal_driver/Inc/Legacy
            ${CMAKE_CURRENT_SOURCE_DIR}/system
            ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/include
            ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Core/include
            ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Core_A/include
            ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Device/ST/STM32H7xx/include
            ${CMAKE_CURRENT_SOURCE_DIR}/startup
            )

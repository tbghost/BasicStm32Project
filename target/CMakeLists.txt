# Define STM32 Target
set(STM32_CHIP STM32H743) # Beispiel: STM32F407 für STM32F4-Serie
set(STM32_HAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/target/hal/HAL_Drivers) # HAL/CMSIS Pfad
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/target/STM32H743ZITX_FLASH.ld)   # Linker-Skript

#
# Core MCU flags, CPU, instruction set and FPU setup
#
set(CPU_PARAMETERS
    -mthumb                 # Thumb-Instruction-Set
    -Wall -Wextra -O0       # optimizations
    -ffreestanding          # independ from Standard-Libraries
    -nostdlib               # none Standard-Startup-Code
    -mcpu=cortex-m7+nofp    # matched Cortex-M Prozessor
	-mfpu=fpv5-d16          # floating arithmetic
	-mfloat-abi=hard        # use floating processor
)

# Baremetal executable
add_executable(stm32_baremetal
    main.cpp
    startup_stm32h743zitx.s
    )

# Include directories
target_include_directories(stm32_baremetal PRIVATE
    ${STM32_HAL_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)


# Compiler and linker flags
target_compile_definitions(stm32_baremetal PRIVATE ${STM32_CHIP})
target_compile_options(stm32_baremetal PRIVATE ${CPU_PARAMETERS})


target_link_options(stm32_baremetal PRIVATE
    -T${LINKER_SCRIPT}      # Linker Script
    -Wl,--gc-sections       # Nicht verwendeten Code entfernen
    -nostartfiles           # Kein Standard-Startup-Code
)

# Optionale STM32-spezifische Bibliotheken hinzufügen
target_sources(stm32_baremetal PRIVATE
    ${STM32_HAL_DIR}/Src/stm32h7xx_hal.c
    ${STM32_HAL_DIR}/Src/stm32h7xx_hal_gpio.c
)
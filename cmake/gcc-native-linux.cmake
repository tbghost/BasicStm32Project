# STM32 Toolchain File
# Dieses Toolchain-File richtet die Umgebung für das STM32-Target ein.
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # <- workaround for bug in cmake 3.16.xx

# Compiler und Toolchain-Spezifikationen
#set(CMAKE_SYSTEM_NAME Generic)                 # Kein Betriebssystem (bare-metal)
#set(CMAKE_SYSTEM_PROCESSOR cortex-m7+nofp)                # Zielprozessor ist ARM
#set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # Verhindert, dass CMake ausführbare Dateien testet

# Pfad zur ARM-Toolchain (anpassen, falls nötig)
set(TOOLCHAIN_PREFIX )

# Compiler- und Linker-Binaries
#set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
#set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-as)
#set(CMAKE_AR ${TOOLCHAIN_PREFIX}-ar)
#set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
#set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)
#set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}-size)

# C und C++ Standard (optional, falls vom Projekt benötigt)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#
# Core MCU flags, CPU, instruction set and FPU setup
#
#set(CPU_PARAMETERS
#    -mcpu=cortex-m7+nofp    # matched Cortex-M Prozessor
#    -mthumb                 # Thumb-Instruction-Set
#    -ffreestanding          # independ from Standard-Libraries
#    -nostdlib               # none Standard-Startup-Code
#    -Og                     # Optimization 
#    -g                      # ""
#    -Wextra                 # ""
#    -Wall                   # ""
#    #-mfpu=fpv5-d16          # floating arithmetic
#	  #-mfloat-abi=hard        # use floating processor
#)

# Compiler-Flags für STM32 (angepasst an Zielarchitektur)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_PARAMETERS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPU_PARAMETERS}")
#set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CPU_PARAMETERS}")

# Linker-Flags
#set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/hal/startup/STM32H743ZITX_FLASH.ld) # Pfad zum Linker-Skript
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_SCRIPT} -Wl,--gc-sections -nostartfiles")


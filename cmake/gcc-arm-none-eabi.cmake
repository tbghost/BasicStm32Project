# STM32 Toolchain File
# Dieses Toolchain-File richtet die Umgebung für das STM32-Target ein.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # <- workaround for bug in cmake 3.16.xx

# Compiler und Toolchain-Spezifikationen
set(CHIP "STM32H743xx")
set(CMAKE_SYSTEM_NAME Generic)                 # Kein Betriebssystem (bare-metal)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7+nofp)     # Zielprozessor ist ARM
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # Verhindert, dass CMake ausführbare Dateien testet


# Pfad zum linkerfile
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/hal/startup/STM32H743ZITX_FLASH.ld)

# Pfad zur ARM-Toolchain (anpassen, falls nötig)
set(TOOLCHAIN_PREFIX arm-none-eabi)

# Compiler- und Linker-Binaries
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc)#as)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}-size)


# Configure flags
set(CFG_SFLAGS
      "EXTERNAL_CLOCK_VALUE=12288000"
	  #"VDD_VALUE=3300"
	  "LSI_VALUE=32000"
	  "HSI_VALUE=64000000"
	  "HSE_VALUE=8000000"
	  "HSE_STARTUP_TIMEOUT=100"
	  "USE_FULL_LL_DRIVER"
	  "DEBUG"
	  "LSE_STARTUP_TIMEOUT=5000"
	  #"${CHIP}"
	  "LSE_VALUE=32768"
        "-DUSE_HAL_DRIVER"
)


###############################################################
# Global compiler options related to debugging and optimization
###############################################################
# CMAKE_BUILD_TYPE=custom_debug
list(APPEND compiler_options_debug   "-O0" "-g" )
list(APPEND compiler_defines_debug   "")

# CMAKE_BUILD_TYPE=custom_release
list(APPEND compiler_options_release "-Os")
list(APPEND compiler_defines_release "NDEBUG")

###############################################################
# Global compiler options related to errors and warnings
###############################################################
list(APPEND compiler_options_warn_and_err_c   "-Wall"                   # enable all warnings
                                              "-Wextra"                 # enable extra warnings
                                              "-Wstrict-prototypes")    # warn if a function is declared or defined without a prototype

list(APPEND compiler_options_warn_and_err_cxx "-Wall"                   # enable all warnings
                                              "-Wextra")                # enable extra warnings

###############################################################
# Global compiler options related to target CPU configuration
###############################################################
list(APPEND target_cpu_conf "-mcpu=cortex-m7+nofp"          # CPU
                            "-mthumb")                      # Thumb-Instruction-Set

list(APPEND target_vfp_conf "-mfloat-abi=soft"              # use software floating point   
                            #"-mfpu=fpv5-d16"               # use floating arithmetic unit             
                            )

# Options for ASM only
list(APPEND compiler_options_asm "SHELL:-x assembler-with-cpp")   # enable preprocessor for ASM files

# Options for C only
list(APPEND compiler_options_c   "")

# Options for C++ only
list(APPEND compiler_options_cxx "-fno-rtti"          #disables Run-Time Type Information (RTTI)
                                 "-fexceptions"       #enables exception handling
                              )     

# Options for all languages (ASM, C, C++)
list(APPEND compiler_options_all "-mabi=aapcs"        # this flag ensures that the generated code adheres to the ARM application binary interface ABI conventions
                                 "-fomit-frame-pointer"     # omit the frame pointer for functions
                                 "-falign-functions=4"      # align functions to 4 bytes (32bit)
                                 "-ffunction-sections"      # place each function or data item into its own section in the output file
                                 "-fdata-sections"          # place each data item into its own section in the output file
                                 "-fno-common"              # initialize uninitialized global variables to zero
                                 "-mno-sched-prolog")       # do not schedule the prologue and epilogue of functions

# newlib specs (all languages and linker)
list(APPEND specs_options        "-specs=nano.specs")       # use the newlib nano specs

# Defines for ASM only
list(APPEND compiler_defines_asm "")

# Defines for C only
list(APPEND compiler_defines_c   "")

# Defines for C++ only
list(APPEND compiler_defines_cxx "")

# Defines for all languages (ASM, C, C++)
list(APPEND compiler_defines_all "${CFG_FLAGS}"
                                 "${CHIP}")

# Options for the linker
list(APPEND linker_options       "-Wl,--gc-sections"        # remove unused sections
                                 "-Wl,--build-id=none"      # do not generate a build-id 
                                 #"-nostartfiles"           # use standard startfiles _linker_start   
                                 #"-nostdlib"               # use standard libraries memcopy
                                 "-static"                  # link statically
                                 "-T${LINKER_SCRIPT}")     
# ---------------------------------------------------------------------------------------------------------------------
# Apply configuration
# ---------------------------------------------------------------------------------------------------------------------
add_compile_options("${target_cpu_conf}"
                    "${target_vfp_conf}"
                    "$<$<CONFIG:CUSTOM_DEBUG>:${compiler_options_debug}>"
                    "$<$<NOT:$<CONFIG:CUSTOM_DEBUG>>:${compiler_options_release}>"
                    "$<$<COMPILE_LANGUAGE:ASM>:${compiler_options_asm}>"
                    "$<$<COMPILE_LANGUAGE:C>:${compiler_options_c}>"
                    "$<$<COMPILE_LANGUAGE:C>:${compiler_options_warn_and_err_c}>"
                    "$<$<COMPILE_LANGUAGE:CXX>:${compiler_options_cxx}>"
                    "$<$<COMPILE_LANGUAGE:CXX>:${compiler_options_warn_and_err_cxx}>"
                    "${compiler_options_all}"
                    "${specs_options}"
                    )

add_compile_definitions("$<$<CONFIG:CUSTOM_DEBUG>:${compiler_defines_debug}>"
                        "$<$<NOT:$<CONFIG:CUSTOM_DEBUG>>:${compiler_defines_release}>"
                        "$<$<COMPILE_LANGUAGE:ASM>:${compiler_defines_asm}>"
                        "$<$<COMPILE_LANGUAGE:C>:${compiler_defines_c}>"
                        "$<$<COMPILE_LANGUAGE:CXX>:${compiler_defines_cxx}>"
                        "${compiler_defines_all}")

add_link_options("${target_cpu_conf}"
                 "${target_vfp_conf}"
                 "${linker_options}"
                 "${specs_options}"
                 "${CFG_FLAGS}"
                 )

    
set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

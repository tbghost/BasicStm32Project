# ================================================================================
# CMake Listfile root/src/utils
# ================================================================================

# add components as library
add_library(Utils 
            STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/IpAddressV4.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
            )



# add Includes to library
target_include_directories(Utils
            PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}
            )


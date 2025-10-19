/**
 ********************************************************************************
 * @file        SysTypes.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils, System type definitions
 * 
 * @author      toberg
 * 
 * @date        2025/10/28
 ********************************************************************************
 */   

#pragma once

#include <cstdint>
namespace Utils::SysTypes {

    /// @brief  Max. integer value on this 32 bit system  
    static constexpr int32_t INT_MAX32 {0x7FFFFFFF};
    
    /** @brief Using unsigned integer literals on this system:
     *  -   using unsigned integer literal 'u' when the value is smaller than or equal the @ref INT_MAX32 value 
     *  -   using unsigned integer literal 'U' when the value is greather than the @ref INT_MAX32 value
     */

    /// @brief  BIGENDIAN flag for this system (0 = little endian = arm default, 1 = big endian)
    static constexpr uint8_t BIGENDIAN{1u};             

} // end namespace Utils::SysTypes
        

/** 
 * @file        C_Startup.hpp
 * 
 * @namespace   HAL
 * 
 * @brief       Startup cpp routines encapsulated in a c++ class.
 * 
 * @author      toberg
 * 
 * @date        2026/01/14
*/
#pragma once

#include <cstdint>
    

namespace HAL {

    
/**
* @brief This class provides startup routines .
*
* - - -
*
* __Thread safety:__
* The access to this class is not safe.
*
*/ 

class C_StartUp 
{
    public:

        /**
        * @fn C_StartUp::Reset_Handler
        * @brief Reset handler function. Called via IRQ vector on every system reset.
        */
        static void Reset_Handler();
       
    
    private:
        /**
        * @brief Constructor not needed because its a static class.
        */
        C_StartUp() = delete;
        
        /**
        * @brief Destructor not needed because its a static class.
        */
        ~C_StartUp() = delete;

        /**
        * @fn C_StartUp::InitDataSegment
        * @brief Initializes the .data segment by copying from flash to RAM.
        */
        static void InitDataSegment();

        /**
        * @fn C_StartUp::InitBssSegment
        * @brief Initializes the .bss segment by zeroing it.
        */
        static void InitBssSegment();

};   // end of class

} // end namespace HAL

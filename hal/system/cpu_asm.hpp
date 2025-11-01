/**
 ********************************************************************************
 * @file        cpu_asm.hpp
 * 
 * @namespace   HAL
 * 
 * @brief       Basic asm routines encapsulated in a c++ class.
 * 
 * @author      toberg
 * 
 * @date        2025/10/28
********************************************************************************/
#pragma once

#include <cstdint>


namespace HAL {

    
/**
* @brief This class provides basic assembler commands .
*
* - - -
*
* __Thread safety:__
* The access to this class is not safe.
*
*/ 

class CPUASM 
{
    public:

        /**
        * @fn CPUASM::DSB
        * @brief Data Synchronization Barrier. Ensure the completion of memory accesses,
        */
        static inline void DSB()
        {
            asm volatile ("DSB");
        };

        /**
        * @fn CPUASM::ISB
        * @brief Instruction Synchronization Barrier. Flush processor pipeline and branch prediction logic.
        */
        static inline void ISB()
        {
            asm volatile ("ISB");
        };

        /**
        * @fn CPUASM::DMB
        * @brief Data Memory Barrier. Ensure the order of observation of memory accesses.
        */
        static inline void DMB()
        {
            asm volatile ("DMB");
        };

        /**
        * @fn CPUASM::NOP
        * @brief No operation.
        */
        static void NOP()
        {
            asm volatile ("NOP");
        };

        /**
        * @fn CPUASM::BRKPT
        * @brief Breakpoint operation.
        */
        static void BRKPT()
        {
            asm volatile ("BKPT #0");
        };

        /**
        * @fn CPUASM::DSI
        * @brief Disables interrupts except NMI and get back previous interrupt states.\n
        *
        * @return Previous interrupt state.\n
        */
        static uint32_t DSI(void)
        {
            uint32_t int_status;
            asm volatile ("MRS %0, PRIMASK" : "=r" (int_status));
            asm volatile ("CPSID i");
            return int_status;
        };

        /**
        * @fn CPUASM::ENI
        * @brief Enables interrupts previously disabled by DSI.
        * 
        * @param int_status Previous interrupt state.\n
        */
        static void ENI(uint32_t int_status)
        {
            asm volatile ("MSR PRIMASK, %0" : : "r" (int_status));
            asm volatile ("CPSIE i");
        };

        /**
        * @fn CPUASM::WFI
        * @brief Waiting for an interrupt.
        */
        static void WFI()
        {
            asm volatile ("WFI");
        };
        
        /**
        * @fn CPUASM::REVERSE32
        * @brief Reverse byte order of a 32bit value from little endian to big endian or vice versa.
        * 
        * @param rValue Reference to the value.\n
        */
        static inline void REVERSE32(uint32_t& rValue)
        {
            asm volatile ("REV %0,%1" : "=r" (rValue) : "r" (rValue));
        };


        /**
        * @fn CPUASM::REVERSE16
        * @brief Reverse byte order of a 16bit value from little endian to big endian or vice versa.
        * 
        * @param rValue Reference to the value.\n
        */
        static inline void REVERSE16(uint16_t& rValue)
        {
            asm volatile ("REVSH %0,%1" : "=r" (rValue) : "r" (rValue));
        };

};   // end of class

} // end namespace HAL

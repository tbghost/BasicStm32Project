/** 
 * @file        C_Startup.cpp
 * 
 * @namespace   HAL
 * 
 * @brief       Startup cpp routines encapsulated in a c++ class.
 * 
 * @author      toberg
 * 
 * @date        2026/01/14
*/


#include "C_Startup.hpp"
#include "cpu_asm.hpp"   
//#include "stm32h7xx.h"
//#include "error_handler.h"

using HAL::C_StartUp;

// Functions called from native C.
extern "C" {
    void IsrReset(void);           // Referenced from reset vector.
    void __libc_init_array(void);   // This symbol is exported by linker, stdlib
}


// Imported Linker symbols
//extern "C" {
    extern uint32_t _sidata;   // Start address for the initialization values of the .data section
    extern uint32_t _sdata;    // Start address for the .data section
    extern uint32_t _edata;    // End address for the .data section
    extern uint32_t _sbss;     // Start address for the .bss section
    extern uint32_t _ebss;     // End address for the .bss section
    extern uint32_t _estack;   // Stack pointer
//}

//void* __dso_handle;  

/**
 * @fn C_StartUp::IsrReset
 * @brief Global entry point, required by the linker script.
 * @note Do not move to any namespace !!
 */
__attribute__((noinline, used))
void IsrReset(void)
{
    __asm volatile ("nop");
    HAL::C_StartUp::Reset_Handler();
};



/**
 * @copydoc C_StartUp::Reset_Handler
 */
void HAL::C_StartUp::Reset_Handler()
{
    // Call standard initialization
    //SystemInit();
    //__libc_init_array();

    // Init FPU if needed
    // FPU activation handled below (Cortex-M7 compliant)
    // #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    //     /* FPU activation according to ARM Cortex-M7 documentation:
    //      * Enable CP10 and CP11 full access in CPACR and ensure the write completes
    //      * before any floating point instructions are executed.
    //      */
    //     SCB->CPACR |= ((3UL << (10*2)) | (3UL << (11*2))); /* set CP10 and CP11 Full Access */

    //     /* Ensure side effects of CPACR write are visible to subsequent instructions */
    //     __DSB();
    //     __ISB();
    // #endif

    //main();

}

/**
 * @copydoc C_StartUp::InitDataSegment
 */
void C_StartUp::InitDataSegment()
{
 //   uint32_t* src = &_sidata;
 //   uint32_t* dest = &_sdata;
 //   uint32_t* end = &_edata;
//
 //   while (dest < end) {
 //       *dest++ = *src++;
 //   }
}

/**
 * @copydoc C_StartUp::InitBssSegment
 */
void C_StartUp::InitBssSegment()
{
  //  uint32_t* start = &_sbss;
  //  uint32_t* end = &_ebss;
//
  //  while (start < end) {
  //      *start++ = 0;
  //  }
}

extern "C" void __aeabi_atexit(void*, void (*)(void*), void*)   // NOSONAR [vschwe]: The function name must be __aeabi_atexit() (otherwise this optimization would be meaningless).
{
    // Nothing to do.
}



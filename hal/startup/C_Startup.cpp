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
#include "IRQ_Handlers.hpp"
#include "stm32h7xx.h"

using HAL::C_StartUp;
using HAL::CPUASM;


// Functions called from native C.
extern "C" {
    void Reset_Handler(void);           // Referenced from reset vector.
}


extern uint32_t _sidata;   // Start address for the initialization values of the .data section
extern uint32_t _sdata;    // Start address for the .data section
extern uint32_t _edata;    // End address for the .data section
extern uint32_t _sbss;     // Start address for the .bss section
extern uint32_t _ebss;     // End address for the .bss section


/**
 * @fn C_StartUp::Reset_Handler
 * @brief Global entry point, required by the linker script.
 * @note Do not move to any namespace !!
 */
//__attribute__((noinline, used))
void Reset_Handler(void)
{
    CPUASM::DSI();
    HAL::C_StartUp::System_Init();
};



/**
 * @copydoc C_StartUp::Reset_Handler
 */
void HAL::C_StartUp::System_Init()
{
    uint32_t tmpReg{0};
    // Call standard initialization
    
    /* FPU activation according to ARM Cortex-M7 documentation:
     * Enable CP10 and CP11 full access in CPACR and ensure the write completes
     * before any floating point instructions are executed.
    */
    SCB->CPACR |= ((3UL << (10*2)) | (3UL << (11*2))); /* set CP10 and CP11 Full Access */
    /* Ensure side effects of CPACR write are visible to subsequent instructions */
    CPUASM::DSB();
    CPUASM::ISB();
    
    InitClk();
    //main();

}



/**
 * @copydoc C_StartUp::InitClk
 */
void C_StartUp::InitClk()
{
    /* Reset the RCC clock configuration to the default reset state */
    RCC->CR |= RCC_CR_HSION;        // Set HSION bit 
    RCC->CFGR = 0x00000000;         // Reset CFGR register 
    RCC->CR &= 0xEAF6ED7FU;         // Reset HSEON, CSSON , CSION,RC48ON, CSIKERON PLL1ON, PLL2ON and PLL3ON bits 
    //RCC->CDCFGR1 = 0x00000000;
    //RCC->CDCFGR2 = 0x00000000;
    //RCC->SRDCFGR = 0x00000000;
    //RCC->SRDCFGR = 0x00000000;

    /* Reset PLL */
    RCC->PLLCKSELR = 0x00000000;
    RCC->PLLCFGR = 0x00000000;      /* Reset PLLCFGR register */
    RCC->PLL1DIVR = 0x00000000;     /* Reset PLL1DIVR register */
    RCC->PLL1FRACR = 0x00000000;    /* Reset PLL1FRACR register */
    RCC->PLL2DIVR = 0x00000000;     /* Reset PLL2DIVR register */
    RCC->PLL2FRACR = 0x00000000;    /* Reset PLL2FRACR register */
    RCC->PLL3DIVR = 0x00000000;     /* Reset PLL3DIVR register */
    RCC->PLL3FRACR = 0x00000000;    /* Reset PLL3FRACR register */
    
    RCC->CR &= 0xFFFBFFFFU;         /* Reset HSEBYP bit */
    RCC->CIER = 0x00000000;         /* Disable all interrupts */
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

extern "C" void __aeabi_atexit(void*, void (*)(void*), void*)   
{
    // Nothing to do.
}



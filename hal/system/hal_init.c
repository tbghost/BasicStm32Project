/*
 * This example shows how application can implement RX and TX DMA for UART.
 * It uses simple packet example approach and 3 separate buffers:
 *
 * - Raw DMA RX buffer where DMA transfers data from UART to memory
 * - Ringbuff for RX data which are transfered from raw buffer and ready for application processin
 * - Ringbuff for TX data to be sent out by DMA TX
 */

/* Includes */
#include "hal_init.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Private function prototypes */
void SystemClock_Config(void);


/**
 * \brief           Application entry point
 */
void hal_init(void) 
{
    //uint8_t state, cmd, len;

    /* MCU Configuration */
//    SCB_DisableDCache();
//    SCB_DisableICache();

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);

    /* Configure the system clock */
    SystemClock_Config();

}


/**
 * \brief           System Clock Configuration
 */
void
SystemClock_Config(void) {
    /* Configure flash latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4) {
        while (1) {}
    }

    /* Configure power supply and voltage scale */
    LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);

    /* Uncomment if used on STM32H745/H755 Nucleo */
    /* Dual-Core Nucleo board used external SMPS instead of LDO */
    /* Manually enable it */
    //PWR->CR3 |= 1 << 2;

    /* Configure HSE */
    LL_RCC_HSE_EnableBypass();
    LL_RCC_HSE_Enable();
    while (!LL_RCC_HSE_IsReady()) {}

    /* Configure PLL */
    LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
    LL_RCC_PLL1P_Enable();
    LL_RCC_PLL1Q_Enable();
    LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
    LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
    LL_RCC_PLL1_SetM(1);
    LL_RCC_PLL1_SetN(120);
    LL_RCC_PLL1_SetP(2);
    LL_RCC_PLL1_SetQ(20);
    LL_RCC_PLL1_SetR(2);
    LL_RCC_PLL1_Enable();
    while (!LL_RCC_PLL1_IsReady()) {}

    /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
    LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
    LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
    LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

    /* Configure systick */
    LL_Init1msTick(480000000);
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
    LL_SetSystemCoreClock(480000000);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);
}

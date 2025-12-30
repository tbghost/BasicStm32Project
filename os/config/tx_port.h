/**
 * @file        tx_port.h
 * 
 * @namespace   
 * 
 * @brief       configuration of threadx os - minimal C++ wrapper
 * 
 * @author      toberg
 * 
 * @date        2025/12/26
*/

#pragma once

#include "stm32h7xx.h"

/* Architecture */
#define TX_PORT_7
#define TX_CPU_ARM_CORTEX_M7

/* Interrupt control */
#define TX_DISABLE_INTERRUPTS()   __disable_irq()
#define TX_RESTORE_INTERRUPTS()   __enable_irq()

/* Stack alignment */
#define TX_STACK_ALIGN            8

/* Tick source */
#define TX_TIMER_USE_SYSTICK
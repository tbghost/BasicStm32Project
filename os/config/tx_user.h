/**
 * @file        tx_user.h
 * 
 * @namespace   
 * 
 * @brief       configuration of threadx os
 * 
 * @author      toberg
 * 
 * @date        2025/12/26
*/

#pragma once

/* Kernel configuration */
#define TX_MAX_PRIORITIES 32                //!< Maximum number of thread priorities, 0 is the highest priority
#define TX_MINIMUM_STACK 256                //!< Minimum stack size for threads
#define TX_TIMER_TICKS_PER_SECOND 1000      //!< 1000 ticks per second

/* Timer thread configuration */
#define TX_TIMER_THREAD_STACK_SIZE 2048     //!< Timer thread stack size
#define TX_TIMER_THREAD_PRIORITY (0)        //!< Timer thread gets the Highest priority

#define TX_ENABLE_ERROR_CHECKING            //!< Enable ThreadX error checking
#define TX_ENABLE_STACK_CHECKING            //!< Enable stack checking by os
#define TX_ENABLE_STACK_FILLING             //!< Enable stack filling by os
#define TX_ENABLE_EVENT_TRACE               //!< Enable event trace by os   
#define TX_ENABLE_PROFILING                 //!< Enable profiling by os
#define TX_DISABLE_NOTIFY_CALLBACKS         //!< Disable notify callbacks to reduce code size
#define TX_BYTE_POOL_ENABLE_PERFORMANCE_INFO//!< Enable byte pool performance info
#define TX_ENABLE_PREEMPTION_THRESHOLD      //!< Enable preemption threshold feature

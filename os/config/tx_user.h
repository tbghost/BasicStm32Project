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

#define TX_MAXPRIORITIES 32                 //!< Maximum number of thread priorities, 0 is the highest priority
#define TX_MINIMUM_STACK_SIZE 2048          //!< Minimum stack size for threads
#define TX_TIMER_THREAD_STACK_SIZE 2048     //!< Timer thread stack size
#define TX_TIMER_THREAD_PRIORITY (0)        //!< Highest priority
#define TX_TIMER_TICKS_PER_SECOND 1000      //!< 1000 ticks per second

#define TX_ENABLE_STACK_CHECKING            //!< Enable stack checking by os
#define TX_ENABLE_STACK_FILLING             //!< Enable stack filling by os
#define TX_ENABLE_EVENT_TRACE               //!< Enable event trace by os   
#define TX_ENABLE_PROFILING                 //!< Enable profiling by os

#define TX_DISABLE_NOTIFY_CALLBACKS         //!< Disable notify callbacks to reduce code size


/**
 * @file        threadx_app.cpp
 * 
 * @brief       ThreadX application entry point and main thread
 * 
 * @author      toberg
 * 
 * @date        2025/12/30
*/

#include "tx_api.h"
#include "tx_port.h"

/**
 * @brief Main application thread function
 * Runs the main application logic in ThreadX context
 */
extern "C" void main_app_thread_entry(ULONG thread_input)
{
    (void)thread_input;  /* Unused parameter */
    
    /* Main application loop */
    while(1)
    {
        /* Application code runs here */
        /* For example: blink LED, process data, etc. */
        
        /* Yield to other threads */
        tx_thread_sleep(100);  /* Sleep for 100ms (100 ticks at 1000 ticks/sec) */
    }
}

/**
 * @brief ThreadX application definition
 * This function is called by ThreadX kernel during initialization
 */
extern "C" void tx_application_define(void *first_unused_memory)
{
    (void)first_unused_memory;  /* Unused parameter */
    
    /* Create the main application thread */
    tx_thread_create(
        &mainAppThread,                          /* Thread control block */
        "Main App Thread",                       /* Thread name */
        main_app_thread_entry,                   /* Thread entry point */
        0,                                       /* Thread input */
        mainAppStack,                            /* Stack pointer */
        TX_APP_STACK_SIZE,                       /* Stack size */
        16,                                      /* Priority (0-31, lower is higher) */
        16,                                      /* Preempt threshold */
        TX_NO_TIME_SLICE,                        /* No time slicing */
        TX_AUTO_START                            /* Auto-start the thread */
    );
}

#include <stdint.h>
#include "hal_init.h"

extern "C" {
#include "tx_api.h"

/* ThreadX low-level initialization function */
void _tx_initialize_low_level(void)
{
    /* Disable interrupts during ThreadX initialization */
    __asm__ volatile("CPSID i");
    
    /* Provide system stack pointer initialization if needed */
    /* The ThreadX library will handle the rest */
}

/**
 * @brief Main application thread function
 * Runs the main application logic in ThreadX context
 */
void main_app_thread_entry(ULONG thread_input)
{
    (void)thread_input;
    
    /* Main application loop */
    while(1)
    {
        /* Application code runs here */
        /* For example: blink LED, process data, etc. */
        
        /* Yield to other threads */
        tx_thread_sleep(100);
    }
}

/**
 * @brief ThreadX application definition
 * This function is called by ThreadX kernel during initialization
 */
void tx_application_define(void *first_unused_memory)
{
    static TX_THREAD mainAppThread;
    static uint8_t mainAppStack[2048];
    
    (void)first_unused_memory;
    
    /* Create the main application thread */
    tx_thread_create(
        &mainAppThread,                          /* Thread control block */
        "Main App Thread",                       /* Thread name */
        main_app_thread_entry,                   /* Thread entry point */
        0,                                       /* Thread input */
        mainAppStack,                            /* Stack pointer */
        sizeof(mainAppStack),                    /* Stack size */
        16,                                      /* Priority */
        16,                                      /* Preempt threshold */
        TX_NO_TIME_SLICE,                        /* No time slicing */
        TX_AUTO_START                            /* Auto-start */
    );
}

}

int main()
{
  /* Initialize HAL */
  hal_init();
  
  /* Enter ThreadX kernel */
  tx_kernel_enter();
  
  /* This line should never be reached */
  return 0;
}
//void Error_Handler() {
//    while (1) {
//        // Fehlerbehandlung
//    }
//}
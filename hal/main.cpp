#include <stdint.h>
#include "hal_init.h"

extern "C" {
#include "tx_api.h"
#include "posix.hpp"

/* ThreadX low-level initialization function */
void _tx_initialize_low_level(void)
{
    /* Disable interrupts during ThreadX initialization */
    __asm__ volatile("CPSID i");
    
    /* Provide system stack pointer initialization if needed */
    /* The ThreadX library will handle the rest */
}

/**
 * @brief Main application thread function using POSIX interface
 * Runs the main application logic in ThreadX context
 */
void main_app_thread_entry(void* arg)
{
    (void)arg;
    
    /* Main application loop */
    while(1)
    {
        /* Application code runs here */
        /* For example: blink LED, process data, etc. */
        
        /* Yield to other threads - POSIX compatible sleep */
        usleep(100000);  /* 100 ms in microseconds */
    }
}

/**
 * @brief ThreadX application definition
 * This function is called by ThreadX kernel during initialization
 */
void tx_application_define(void *first_unused_memory)
{
    static pthread_t mainAppThread;
    
    (void)first_unused_memory;
    
    /* Create the main application thread using POSIX interface */
    pthread_create(
        &mainAppThread,           /* Thread handle */
        NULL,                     /* Thread attributes (not used) */
        main_app_thread_entry,    /* Thread entry point */
        NULL                      /* Thread argument */
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
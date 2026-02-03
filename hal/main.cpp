#include <stdint.h>
#include "hal_init.h"
#include "Logger.hpp"
#include "UartLogger.hpp"

extern "C" {
#include "tx_api.h"
#include "posix.hpp"

using namespace Utils;

/**
 * @brief Main application thread function using POSIX interface
 * Runs the main application logic in ThreadX context
 */
void main_app_thread_entry(void* arg)
{
    (void)arg;
    
    int iteration = 0;
    
    /* Main application loop */
    while(1)
    {
        /* Process logger messages */
        UartLogger::processMessages();
        
        /* Application code runs here */
        iteration++;
        Logger::info("Main thread running...");
        
        /* Yield to other threads - POSIX compatible sleep */
        int ret = usleep(100000);  /* 100 ms in microseconds */
        if (ret != 0) {
            Logger::error("usleep failed");
            error_handler_reset(ERROR_THREADX_INIT_FAILED);
        }
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
  
  /* Initialize Logger with UART output */
  UartLogger::init();
  Logger::info("ThreadX starting...");
  
  /* Enter ThreadX kernel */
  tx_kernel_enter();
  
  /* If tx_kernel_enter() returns, something went wrong */
  Logger::critical("ThreadX kernel exited!");
  error_handler_reset(ERROR_THREADX_INIT_FAILED);
  
  /* This line should never be reached */
  return 0;
}

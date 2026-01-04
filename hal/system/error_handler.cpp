/**
 * @file error_handler.cpp
 * @brief Error handling implementation with Software Reset (C++ version)
 */

#include "error_handler.h"
#include "stm32h7xx.h"
#include <cstdio>
#include <cstdint>

/**
 * @brief Backup register to store error code across reset
 * Using STM32H7 RTC backup register (can persist across reset)
 */
#define ERROR_BACKUP_REGISTER RTC->BKP0R

/**
 * @brief System error counter (volatile to prevent optimization)
 */
static volatile uint32_t error_count = 0;

/**
 * @brief Maximum number of reset attempts before hard fault
 */
constexpr uint32_t MAX_RESET_ATTEMPTS = 5;

/**
 * @brief AIRCR register key value for ARM Cortex-M
 */
constexpr uint32_t AIRCR_VECTKEY = 0x5FA;

/**
 * @class ErrorHandler
 * @brief Singleton class for error handling
 */
class ErrorHandler {
private:
    static bool initialized;
    
    ErrorHandler() = default;
    
public:
    // Delete copy and move constructors
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;
    ErrorHandler(ErrorHandler&&) = delete;
    ErrorHandler& operator=(ErrorHandler&&) = delete;
    
    /**
     * @brief Get the singleton instance
     */
    static ErrorHandler& getInstance() {
        static ErrorHandler instance;
        return instance;
    }
    
    /**
     * @brief Initialize error handler
     */
    void init() {
        if (initialized) {
            return;
        }
        
        /* Clear error counter on initialization */
        error_count = 0;
        
        initialized = true;
    }
    
    /**
     * @brief Handle error and trigger software reset
     */
    void handleError(error_code_t error_code) {
        /* Increment error counter to detect repeated resets */
        error_count++;
        
        /* Prevent infinite reset loops */
        if (error_count > MAX_RESET_ATTEMPTS) {
            /* If we keep resetting, enter infinite loop instead */
            __asm__ volatile("BKPT #0");
            while (true) {
                /* Hard fault - requires manual intervention */
            }
        }
        
        /* Store error code in backup register for post-reset analysis */
        ERROR_BACKUP_REGISTER = static_cast<uint32_t>(error_code);
        
        /* Log the error if debugging is enabled */
        logError(error_code);
        
        /* Flush any pending data */
        flushOutput();
        
        /* Trigger system-wide software reset */
        triggerReset();
        
        /* This line should never be reached */
        while (true) {}
    }
    
    /**
     * @brief Trigger immediate software reset
     */
    void triggerReset() {
        /* Ensure interrupts are disabled before reset */
        __asm__ volatile("CPSID i");
        
        /* Request system reset using AIRCR */
        SCB->AIRCR = static_cast<uint32_t>(
            (AIRCR_VECTKEY << SCB_AIRCR_VECTKEY_Pos)    /* VECTKEY (write key) */
            | SCB_AIRCR_SYSRESETREQ_Msk                 /* SYSRESETREQ bit */
        );
        
        /* Wait for reset to take effect */
        while (true) {
            __asm__ volatile("NOP");
        }
    }
    
    /**
     * @brief Get the last error code
     */
    error_code_t getLastError() {
        /* Read error code from backup register */
        error_code_t last_error = static_cast<error_code_t>(ERROR_BACKUP_REGISTER & 0xFF);
        
        /* Clear the backup register after reading */
        ERROR_BACKUP_REGISTER = 0;
        
        return last_error;
    }
    
private:
    /**
     * @brief Log error information
     */
    void logError(error_code_t error_code) {
        #if defined(DEBUG) || defined(ENABLE_ERROR_LOGGING)
        const char* error_message = getErrorMessage(error_code);
        printf("ERROR: Code 0x%02X - %s - Initiating Software Reset\r\n", 
               static_cast<uint8_t>(error_code), error_message);
        #else
        (void)error_code;  /* Suppress unused warning in release build */
        #endif
    }
    
    /**
     * @brief Get error message string
     */
    const char* getErrorMessage(error_code_t error_code) const {
        switch (error_code) {
            case ERROR_HAL_INIT_FAILED:
                return "HAL Initialization Failed";
            case ERROR_THREADX_INIT_FAILED:
                return "ThreadX Initialization Failed";
            case ERROR_THREAD_CREATE_FAILED:
                return "Thread Creation Failed";
            case ERROR_MEMORY_ALLOCATION:
                return "Memory Allocation Failed";
            case ERROR_WATCHDOG_RESET:
                return "Watchdog Triggered";
            case ERROR_ASSERTION_FAILED:
                return "Assertion Failed";
            case ERROR_SYSTEM_CRITICAL:
                return "Critical System Error";
            default:
                return "Unknown Error";
        }
    }
    
    /**
     * @brief Flush output buffers
     */
    void flushOutput() {
        /* Wait for UART TX to complete */
        for (volatile int i = 0; i < 10000; i++) {}
    }
};

/* Initialize static member */
bool ErrorHandler::initialized = false;

/* ==================== C Interface Wrappers ==================== */

extern "C" {

/**
 * @brief Initialize the error handler module
 */
void error_handler_init(void)
{
    ErrorHandler::getInstance().init();
}

/**
 * @brief Handle error and trigger software reset
 */
void error_handler_reset(error_code_t error_code)
{
    ErrorHandler::getInstance().handleError(error_code);
}

/**
 * @brief Trigger immediate software reset
 */
void system_software_reset(void)
{
    ErrorHandler::getInstance().triggerReset();
}

/**
 * @brief Get the last error code that caused a reset
 */
error_code_t error_handler_get_last_error(void)
{
    return ErrorHandler::getInstance().getLastError();
}

/**
 * @brief Assert handler - called by assert macro
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;  /* Unused in minimal implementation */
    (void)line;
    
    #if defined(USE_FULL_ASSERT)
    /* Trigger error handler with assertion failed code */
    error_handler_reset(ERROR_ASSERTION_FAILED);
    #endif
}

/**
 * @brief Hard Fault Handler
 */
void HardFault_Handler(void)
{
    error_handler_reset(ERROR_SYSTEM_CRITICAL);
    while (true) {}
}

/**
 * @brief Memory Management Fault Handler
 */
void MemManage_Handler(void)
{
    error_handler_reset(ERROR_MEMORY_ALLOCATION);
    while (true) {}
}

/**
 * @brief Bus Fault Handler
 */
void BusFault_Handler(void)
{
    error_handler_reset(ERROR_SYSTEM_CRITICAL);
    while (true) {}
}

/**
 * @brief Usage Fault Handler
 */
void UsageFault_Handler(void)
{
    error_handler_reset(ERROR_SYSTEM_CRITICAL);
    while (true) {}
}

} /* extern "C" */

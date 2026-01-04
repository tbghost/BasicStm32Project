/**
 * @file error_handler.h
 * @brief Error handling with Software Reset for STM32H7
 * @details Provides error handling functions that perform system-wide resets
 */

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Error codes for different error conditions
 */
typedef enum {
    ERROR_HAL_INIT_FAILED = 0x01,      /**< HAL initialization failed */
    ERROR_THREADX_INIT_FAILED = 0x02,  /**< ThreadX initialization failed */
    ERROR_THREAD_CREATE_FAILED = 0x03, /**< Thread creation failed */
    ERROR_MEMORY_ALLOCATION = 0x04,    /**< Memory allocation failed */
    ERROR_WATCHDOG_RESET = 0x05,       /**< Watchdog triggered reset */
    ERROR_ASSERTION_FAILED = 0x06,     /**< Assertion failed */
    ERROR_SYSTEM_CRITICAL = 0xFF       /**< Critical system error */
} error_code_t;

/**
 * @brief Initialize the error handler module
 * @details Sets up error handling infrastructure (logging, monitoring, etc.)
 */
void error_handler_init(void);

/**
 * @brief Handle error and trigger software reset
 * @param error_code The error code indicating what went wrong
 * @details This function will:
 *          1. Log the error (if logging is available)
 *          2. Perform cleanup if needed
 *          3. Trigger a system-wide software reset
 */
void error_handler_reset(error_code_t error_code);

/**
 * @brief Trigger immediate software reset
 * @details Uses ARM Cortex-M7 AIRCR register to trigger immediate reset
 *          This bypasses any error logging
 */
void system_software_reset(void);

/**
 * @brief Get the last error code that caused a reset
 * @return The error code from the last reset (if recoverable)
 * @details This can be read from persistent storage or BACKUP registers
 */
error_code_t error_handler_get_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* ERROR_HANDLER_H */

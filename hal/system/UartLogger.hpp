/**
 ********************************************************************************
 * @file        UartLogger.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       UART output adapter for Logger (ST-Link UART 115200 baud)
 * 
 * @author      toberg
 * 
 * @date        2026/01/31
 ********************************************************************************/

#pragma once

#include "Logger.hpp"
#include <cstdint>

namespace Utils {

/**
 * @class UartLogger
 * @brief Outputs buffered logger messages to ST-Link UART (115200 baud)
 * 
 * This class reads messages from the lock-free Logger ringbuffer and
 * outputs them to USART3 (ST-Link on STM32H743).
 * 
 * Usage:
 *  - Call UartLogger::init() once at startup
 *  - Call UartLogger::processMessages() periodically (e.g., in main loop or timer)
 *  - Logger::debug/info/warn/error/critical() write to the ringbuffer
 *  - UartLogger outputs to UART asynchronously
 */
class UartLogger {
public:
    /// @brief Initialize UART (115200 baud, USART3)
    static void init();
    
    /// @brief Process and output all pending logger messages
    /// @details Should be called periodically from normal context (not ISR)
    static void processMessages();
    
    /// @brief Print formatted message to UART
    /// @param format Printf-style format string
    static void printf(const char* format, ...);
    
private:
    /// @brief Initialize USART3 (ST-Link UART)
    static void initUart();
    
    /// @brief Send single character to UART
    static void putchar(char c);
    
    /// @brief Send string to UART
    static void puts(const char* str);
    
    /// @brief Format log level as string
    static const char* levelToString(LogLevel level);
};

}  // namespace Utils

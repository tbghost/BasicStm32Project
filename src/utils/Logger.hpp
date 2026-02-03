/**
 ********************************************************************************
 * @file        Logger.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       Lock-free ISR-safe Logger with atomic ringbuffer
 * 
 * @author      toberg
 * 
 * @date        2026/01/31
 ********************************************************************************/

#pragma once

#include <atomic>
#include <cstdint>
#include <array>
#include <cstring>

namespace Utils {

/**
 * @enum LogLevel
 * @brief Log severity levels
 */
enum class LogLevel : uint8_t {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    CRITICAL = 4
};

/**
 * @struct Message
 * @brief Single log message in the ringbuffer
 */
struct Message {
    static constexpr uint16_t MAX_TEXT_LEN = 256;
    
    LogLevel level;
    uint32_t timestamp;
    char text[MAX_TEXT_LEN];
    
    Message() : level(LogLevel::INFO), timestamp(0) {
        text[0] = '\0';
    }
};

/**
 * @class Logger
 * @brief Lock-free, ISR-safe logger using atomic ringbuffer
 * 
 * Features:
 * - No mutexes required (ISR-safe)
 * - Atomic read/write pointers
 * - Overflow detection via atomic flag
 * - Static ringbuffer (no dynamic allocation)
 * - Suitable for both normal code and ISR context
 * 
 * Thread Safety:
 * - Write operations are safe from ISR context
 * - Read operations can be performed from normal context
 * - Overflow flag indicates lost messages
 */
class Logger {
public:
    static constexpr uint16_t BUFFER_SIZE = 128;  // Number of messages
    
    /// @brief Initialize the singleton logger
    static void init();
    
    /// @brief Get singleton instance
    static Logger& getInstance();
    
    /// @brief Write debug message (ISR-safe)
    static void debug(const char* text);
    
    /// @brief Write info message (ISR-safe)
    static void info(const char* text);
    
    /// @brief Write warning message (ISR-safe)
    static void warn(const char* text);
    
    /// @brief Write error message (ISR-safe)
    static void error(const char* text);
    
    /// @brief Write critical message (ISR-safe)
    static void critical(const char* text);
    
    /// @brief Read next message from buffer (non-ISR context only)
    /// @return true if message was read, false if buffer empty
    bool readMessage(Message& msg);
    
    /// @brief Get current fill level (0-100%)
    uint8_t getFillLevel() const;
    
    /// @brief Check if overflow occurred
    bool hasOverflowed() const;
    
    /// @brief Clear overflow flag
    void clearOverflowFlag();
    
    /// @brief Get total messages written
    uint32_t getTotalMessagesWritten() const;
    
    /// @brief Get total messages lost due to overflow
    uint32_t getTotalMessagesLost() const;
    
private:
    /// @brief Private constructor
    Logger();
    
    /// @brief Write message to ringbuffer (ISR-safe)
    void writeMessage(LogLevel level, const char* text);
    
    /// Static ringbuffer storage
    static std::array<Message, BUFFER_SIZE> buffer;
    static Logger instance;
    
    /// Atomic pointers for lock-free operation
    std::atomic<uint16_t> writePtr{0};  ///< Write pointer
    std::atomic<uint16_t> readPtr{0};   ///< Read pointer
    
    /// Overflow tracking
    std::atomic<bool> overflowFlag{false};
    std::atomic<uint32_t> totalWritten{0};
    std::atomic<uint32_t> totalLost{0};
    
    /// Deleted copy/move operations
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;
};

}  // namespace Utils
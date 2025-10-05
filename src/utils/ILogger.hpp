/**
 ********************************************************************************
 * @file        ILogger.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils, Interface for logging functionality.
 * 
 * @author      toberg
 * 
 * @date        2025/10/05
********************************************************************************/

#pragma once

#include <string>
#include <cstdint>
namespace Utils {


/**
 * @brief   This class provides a lockfree Logger functionality for using in different contexts of Threads and ISR's.
 * @details The lock messages are stored in a ringbuffer and printed out time uncritical by a dedicated thread.
 * @note    Be sure, inside a ISR use const strings or static allocated strings only !!
 *  - - - 
 * 
 * __Thread safety:__
 * The access to this class is thread safe and ISR safe.\n
 * It's in users responsibility to make sure a quick and clean access on a ISR context.
 *
 */
class ILogger
{
    public:
    
        /// @brief Global used log levels.
        enum class LogLevel : uint8_t 
        { 
            DEBUG=0,  //!< Debug level, detailed information, used only by developers
            INFO=1,   //!< Info level, general operational entries about what's going on inside the application 
            WARN=2,   //!< Warning level, an indication that something unexpected happened, the application is still working as expected.
            ERROR=3   //!< Error level, a serious issue that prevents the application from functioning correctly.
        };

        /// @brief Global used log level to string.
        static const char* ToString(LogLevel lvl)
        {
            switch (lvl)
            {
                case LogLevel::DEBUG: return "DBG";
                case LogLevel::INFO:  return "INF";
                case LogLevel::WARN:  return "WRN";
                case LogLevel::ERROR: return "ERR";
                default: return "UNK";
            }
            return "UNK";
        };


        /// @brief Capacity of the internal ringbuffer in bytes.
        static constexpr size_t BUFFER_CAPACITY = 4096;

        /// @brief Size of the header for each log entry in bytes.
        static constexpr size_t HEADER_SIZE = sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint8_t);

        /**
         * @brief Log a message with a specific log level.
         * @param level The log level of the message.
         * @param msg   The message to log.
         */
        virtual void Log(LogLevel level, const char *msg) = 0;

    protected:

        /// @brief Virtual constructor.
        virtual ILogger() = default;

        /// @brief Virtual destructor for proper cleanup in derived classes.
        virtual ~ILogger() = default;

        ILogger(ILogger const &) = default;             //!< Copy constructor
        ILogger(ILogger &&) = default;                  //!< Move constructor

        ILogger& operator=(ILogger const &) = default;  //!< Copy assignment
        ILogger& operator=(ILogger &&) = default;       //!< Move assignment

};

} // end namespace Utils

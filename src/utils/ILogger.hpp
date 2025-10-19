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
* @brief This class provides a BaseStream on heap by the format: size[3...0]:data[size].
*
* - - -
*
* __Thread safety:__
* The access to this class is not safe.
*
* - - -
*
* 
*/
class BaseStream
{
    public:

        static constexpr uint32_t MAXSTREAMLENGTH{1024U};   //!< Maximum length of a BaseStream in bytes}

        /**
        * @brief This Method creates and get back the ownership of a serialized stream object on heap by the format: size[3...0],data[0...size-1].
        * 
        * - - - 
        * 
        * __Thread safety:__
        * The access to this object is thread safe.
        * 
        * - - - 
        * 
        * @param data Pointer to the BaseStream data
        * @param size Size of the BaseStream data in bytes
        * 
        * @return Unique pointer to the created BaseStream object (moves the ownereship), nullptr if an error occurred.
        */
        static std::unique_ptr<uint8_t[]> CreateObj(uint8_t* const data, size_t const size)
        {
            if ((data != nullptr) && (size > 0) && (size < (MAXSTREAMLENGTH - sizeof(size)) ))
            {
                std::unique_ptr<uint8_t[]> mData{ std::make_unique<uint8_t[]>(size + sizeof(size)) };

                if (mData != nullptr)
                {
                    // first four bytes contains the size of BaseStream
                    mData[0] = static_cast<uint8_t>((size >> 24) & 0xFFU);
                    mData[1] = static_cast<uint8_t>((size >> 16) & 0xFFU);
                    mData[2] = static_cast<uint8_t>((size >> 8) & 0xFFU);
                    mData[3] = static_cast<uint8_t>(size & 0xFFU);
                    // copy the BaseStream
                    for (uint32_t i = 0; i < size; ++i)
                    {
                        mData[i + sizeof(size)] = data[i];
                    }
                    return mData;
                }
                else
                {
                    //this should never happen because the heap is exhausted 
                }
                
            }
            return nullptr;
        };

        /**
        * @brief Default Constructor.
        */ 
        BaseStream() = default;

        BaseStream(BaseStream const&) = delete;              //!< none copy constructor
        BaseStream(BaseStream&&) = delete;                   //!< none move constructor
        
        /**
        * @brief Default Destructor.
        */ 
        ~BaseStream() = default;

        BaseStream& operator=(BaseStream const&) = delete;   //!< none copy operator
        BaseStream& operator=(BaseStream&&) = delete;        //!< none move operator

        /**
        * @brief This Method provides data and size of a referenced BaseStream object.
        * 
        * - - - 
        * 
        * __Thread safety:__
        * The access to this class is thread safe.
        * 
        * - - - 
        * 
        * @param pData Unique_ptr to the refernced BaseStream object
        * 
        * @return std::span of a referenced BaseStream object.
        */
        static std::span<uint8_t> GetObjData(std::unique_ptr<uint8_t[]>& pData) 
        {
            if (pData != nullptr)
            {
                uint32_t size{ 0U };
                size |= static_cast<uint32_t>(pData[0]) << 24;
                size |= static_cast<uint32_t>(pData[1]) << 16;
                size |= static_cast<uint32_t>(pData[2]) << 8;
                size |= static_cast<uint32_t>(pData[3]);
                return std::span<uint8_t>(pData.get() + sizeof(size), size);
            }
            return {};  // empty span
        };
            
};



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
        enum class LogLevel : uint16_t 
        { 
            DEBUG=0,  //!< Debug level, detailed information, used only by developers
            INFO=1,   //!< Info level, general operational entries about what's going on inside the application 
            WARN=2,   //!< Warning level, an indication that something unexpected happened, the application is still working as expected.
            ERROR=3,  //!< Error level, a serious issue that prevents the application from functioning correctly.
            UNK=4     //!< Unknown level, used for uninitialized or unknown log levels
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

        
        /// @brief header of a logmessage.
        struct LogHeader
        {
            uint32_t    id;         //!< ID of the log entry, identifies the ascending order
            uint32_t    timestamp;  //!< Timestamp of the log entry (e.g., milliseconds since start)
            uint16_t    length;     //!< Length of the log message (excluding the header)
            uint16_t    level;      //!< Log level (0=DEBUG, 1=INFO, 2=WARN, 3=ERROR)
        } __attribute__((packed));


        /// @brief Size of the header for each log entry in bytes.
        static constexpr size_t HEADER_SIZE = sizeof(LogHeader);

        /**
         * @brief Log a message with a specific log level.
         * @param level The log level of the message.
         * @param msg   The message to log.
         */
        virtual void Log(LogLevel level, const char *msg) = 0;

    protected:

        /// @brief Virtual constructor.
        ILogger() = default;

        /// @brief Virtual destructor for proper cleanup in derived classes.
        ~ILogger() = default;

        ILogger(ILogger const &) = default;             //!< Copy constructor
        ILogger(ILogger &&) = default;                  //!< Move constructor

        ILogger& operator=(ILogger const &) = default;  //!< Copy assignment
        ILogger& operator=(ILogger &&) = default;       //!< Move assignment

};

} // end namespace Utils

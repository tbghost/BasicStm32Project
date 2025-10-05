/**
 ********************************************************************************
 * @file        Logger.hpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils, logging functionality.
 * 
 * @author      toberg
 * 
 * @date        2025/10/05
********************************************************************************/

#pragma once

#include "ILogger.hpp"
#include <string>
#include <cstdint>
#include <array>
#include <atomic>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <ctime>
namespace Utils {


/**
 * @brief   This class provides a lockfree Logger functionality for using in different contexts of Threads, ISR's.
 * @details The lock messages are stored in a ringbuffer and printed by a dedicated thread.
 * @note    Be sure, inside a ISR use const strings or static allocated strings only !!
 *  - - - 
 * 
 * __Thread safety:__
 * The access to this class is thread safe and ISR safe.\n
 * It's in users responsibility to make sure a quick and clean access on a ISR context.
 *
 */
class Logger
{
public:
    
    /// @brief Logger Constructor
    explicit Logger(LogLevel suppLevel);
        : head(0) 
        , tail(0)
        , running(true)
        , mSuppLevel(suppLevel)
    {
        consoleFd = open("/dev/console", O_RDWR);
        if (consoleFd < 0) consoleFd = 1; // fallback stdout
        pthread_create(&thread, nullptr, &Logger::threadEntry, this);
    }

    ~Logger()
    {
        running = false;
        pthread_join(thread, nullptr);
        if (consoleFd > 2) close(consoleFd);
    }

    /// @brief Log a message with a specific log level.
    void Log(LogLevel level, const char *msg) override
    {
        if (level < minLevel) 
        {
            return;
        }
        const uint16_t len = strlen(msg);
        const uint32_t ts = getTimestamp();
        const uint8_t lvl = static_cast<uint8_t>(level);
        const size_t total = HEADER_SIZE + len;
        if (total >= BUFFER_CAPACITY - 1) return;

        size_t h = head.load(std::memory_order_relaxed);
        size_t t = tail.load(std::memory_order_acquire);

        // Überlaufbehandlung
        while (freeSpace(h, t) <= total)
        {
            uint16_t oldLen;
            readBytes(t, reinterpret_cast<uint8_t*>(&oldLen), sizeof(uint16_t));
            t = (t + HEADER_SIZE + oldLen) % BUFFER_CAPACITY;
        }

        writeBytes(h, reinterpret_cast<const uint8_t*>(&len), sizeof(uint16_t));
        h = (h + sizeof(uint16_t)) % BUFFER_CAPACITY;
        writeBytes(h, reinterpret_cast<const uint8_t*>(&ts), sizeof(uint32_t));
        h = (h + sizeof(uint32_t)) % BUFFER_CAPACITY;
        writeBytes(h, &lvl, sizeof(uint8_t));
        h = (h + sizeof(uint8_t)) % BUFFER_CAPACITY;
        writeBytes(h, reinterpret_cast<const uint8_t*>(msg), len);
        h = (h + len) % BUFFER_CAPACITY;

        head.store(h, std::memory_order_release);
        tail.store(t, std::memory_order_release);
    }

    void setLevel(Level l) { minLevel = l; }
    Level getLevel() const { return minLevel; }

    

private:
    
    /// @brief Internal buffer for log messages.
    std::array<uint8_t, BUFFER_CAPACITY> mBuffer{0};

    /// @brief Head of ringbuffer 
    std::atomic<size_t> mHead;

     /// @brief Tail of ringbuffer.
    std::atomic<size_t>mTail;
    
    /// @brief Thread handling the log output.
    pthread_t mThread;

    /// @brief The supported log level.
    std::atomic<bool> mRunning;
    
    /// @brief The smallest supported log level.
    /// @note The logger supports all Levels >= mSuppLevel
    LogLevel mSuppLevel;

    /// @brief The Thread routine
    /// @param arg 
    static void *ThreadEntry(void *arg);
    {
        static_cast<Logger*>(arg)->process();
        return nullptr;
    }

    void process()
    {
        while (running)
        {
            printNext();
            usleep(2000);
        }
    }

    void printNext()
    {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire);
        if (t == h) return;

        uint16_t len;
        uint32_t ts;
        uint8_t lvl;
        char msg[256];

        readBytes(t, reinterpret_cast<uint8_t*>(&len), sizeof(uint16_t));
        t = (t + sizeof(uint16_t)) % BUFFER_CAPACITY;
        readBytes(t, reinterpret_cast<uint8_t*>(&ts), sizeof(uint32_t));
        t = (t + sizeof(uint32_t)) % BUFFER_CAPACITY;
        readBytes(t, &lvl, sizeof(uint8_t));
        t = (t + sizeof(uint8_t)) % BUFFER_CAPACITY;
        if (len >= sizeof(msg)) len = sizeof(msg) - 1;
        readBytes(t, reinterpret_cast<uint8_t*>(msg), len);
        msg[len] = '\0';
        t = (t + len) % BUFFER_CAPACITY;

        dprintf(consoleFd, "[%010u][%s] %s\n", ts, levelToString(static_cast<Level>(lvl)), msg);
        tail.store(t, std::memory_order_release);
    }

    static uint32_t getTimestamp()
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    }

    size_t freeSpace(size_t h, size_t t) const
    {
        return (h >= t) ? BUFFER_CAPACITY - (h - t) : t - h;
    }

    void writeBytes(size_t pos, const uint8_t *data, size_t len)
    {
        for (size_t i = 0; i < len; i++) { buffer[pos] = data[i]; pos = (pos + 1) % BUFFER_CAPACITY; }
    }

    void readBytes(size_t pos, uint8_t *data, size_t len)
    {
        for (size_t i = 0; i < len; i++) { data[i] = buffer[pos]; pos = (pos + 1) % BUFFER_CAPACITY; }
    }
};

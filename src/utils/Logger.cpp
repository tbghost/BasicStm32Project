/**
 ********************************************************************************
 * @file        Logger.cpp
 * 
 * @namespace   Utils
 * 
 * @brief       Implementation of lock-free ISR-safe Logger
 * 
 * @author      toberg
 * 
 * @date        2026/01/31
 ********************************************************************************/

#include "Logger.hpp"
#include <cstring>

namespace Utils {

// Static member initialization
std::array<Message, Logger::BUFFER_SIZE> Logger::buffer;
Logger Logger::instance;

Logger::Logger() 
    : writePtr(0), readPtr(0), overflowFlag(false), totalWritten(0), totalLost(0)
{
}

void Logger::init()
{
    // Singleton initialization - called once at startup
    getInstance();
}

Logger& Logger::getInstance()
{
    return instance;
}

void Logger::debug(const char* text)
{
    getInstance().writeMessage(LogLevel::DEBUG, text);
}

void Logger::info(const char* text)
{
    getInstance().writeMessage(LogLevel::INFO, text);
}

void Logger::warn(const char* text)
{
    getInstance().writeMessage(LogLevel::WARN, text);
}

void Logger::error(const char* text)
{
    getInstance().writeMessage(LogLevel::ERROR, text);
}

void Logger::critical(const char* text)
{
    getInstance().writeMessage(LogLevel::CRITICAL, text);
}

void Logger::writeMessage(LogLevel level, const char* text)
{
    if (!text) {
        return;
    }
    
    // Load current pointers (atomic, acquire semantics not needed for write-only)
    uint16_t wp = writePtr.load(std::memory_order_relaxed);
    uint16_t rp = readPtr.load(std::memory_order_acquire);
    
    // Check if buffer is full
    uint16_t nextWp = (wp + 1) % BUFFER_SIZE;
    if (nextWp == rp) {
        // Buffer full - overflow
        overflowFlag.store(true, std::memory_order_release);
        totalLost.fetch_add(1, std::memory_order_relaxed);
        return;
    }
    
    // Write message to buffer
    Message& msg = buffer[wp];
    msg.level = level;
    msg.timestamp = 0;  // Could use HAL tick counter here
    
    // Copy text safely (limit to MAX_TEXT_LEN)
    uint16_t len = static_cast<uint16_t>(strlen(text));
    if (len >= Message::MAX_TEXT_LEN) {
        len = Message::MAX_TEXT_LEN - 1;
    }
    std::memcpy(msg.text, text, len);
    msg.text[len] = '\0';
    
    // Update write pointer (release semantics to ensure message is visible)
    writePtr.store(nextWp, std::memory_order_release);
    totalWritten.fetch_add(1, std::memory_order_relaxed);
}

bool Logger::readMessage(Message& msg)
{
    uint16_t rp = readPtr.load(std::memory_order_relaxed);
    uint16_t wp = writePtr.load(std::memory_order_acquire);
    
    // Check if buffer is empty
    if (rp == wp) {
        return false;
    }
    
    // Copy message from buffer
    msg = buffer[rp];
    
    // Update read pointer (release semantics)
    readPtr.store((rp + 1) % BUFFER_SIZE, std::memory_order_release);
    return true;
}

uint8_t Logger::getFillLevel() const
{
    uint16_t wp = writePtr.load(std::memory_order_relaxed);
    uint16_t rp = readPtr.load(std::memory_order_relaxed);
    
    uint16_t count;
    if (wp >= rp) {
        count = wp - rp;
    } else {
        count = BUFFER_SIZE - rp + wp;
    }
    
    return static_cast<uint8_t>((count * 100) / BUFFER_SIZE);
}

bool Logger::hasOverflowed() const
{
    return overflowFlag.load(std::memory_order_acquire);
}

void Logger::clearOverflowFlag()
{
    overflowFlag.store(false, std::memory_order_release);
}

uint32_t Logger::getTotalMessagesWritten() const
{
    return totalWritten.load(std::memory_order_relaxed);
}

uint32_t Logger::getTotalMessagesLost() const
{
    return totalLost.load(std::memory_order_relaxed);
}

}  // namespace Utils
//        return;
//    }
//    const uint16_t len = strlen(msg);
//    const uint32_t ts = getTimestamp();
//    const uint8_t lvl = static_cast<uint8_t>(level);
//    const size_t total = HEADER_SIZE + len;
//    if (total >= BUFFER_CAPACITY - 1) return;
//    size_t h = head.load(std::memory_order_relaxed);
//    size_t t = tail.load(std::memory_order_acquire);
//    // Überlaufbehandlung
//    while (freeSpace(h, t) <= total)
//    {
//        uint16_t oldLen;
//        readBytes(t, reinterpret_cast<uint8_t*>(&oldLen), sizeof(uint16_t));
//        t = (t + HEADER_SIZE + oldLen) % BUFFER_CAPACITY;
//    }
//    writeBytes(h, reinterpret_cast<const uint8_t*>(&len), sizeof(uint16_t));
//    h = (h + sizeof(uint16_t)) % BUFFER_CAPACITY;
//    writeBytes(h, reinterpret_cast<const uint8_t*>(&ts), sizeof(uint32_t));
//    h = (h + sizeof(uint32_t)) % BUFFER_CAPACITY;
//    writeBytes(h, &lvl, sizeof(uint8_t));
//    h = (h + sizeof(uint8_t)) % BUFFER_CAPACITY;
//    writeBytes(h, reinterpret_cast<const uint8_t*>(msg), len);
//    h = (h + len) % BUFFER_CAPACITY;
//    head.store(h, std::memory_order_release);
//    tail.store(t, std::memory_order_release);
//}
//
//
//
//    static void *threadEntry(void *arg)
//    {
//        static_cast<Logger*>(arg)->process();
//        return nullptr;
//    }
//
//    void process()
//    {
//        while (running)
//        {
//            printNext();
//            usleep(2000);
//        }
//    }
//
//    void printNext()
//    {
//        size_t t = tail.load(std::memory_order_relaxed);
//        size_t h = head.load(std::memory_order_acquire);
//        if (t == h) return;
//
//        uint16_t len;
//        uint32_t ts;
//        uint8_t lvl;
//        char msg[256];
//
//        readBytes(t, reinterpret_cast<uint8_t*>(&len), sizeof(uint16_t));
//        t = (t + sizeof(uint16_t)) % BUFFER_CAPACITY;
//        readBytes(t, reinterpret_cast<uint8_t*>(&ts), sizeof(uint32_t));
//        t = (t + sizeof(uint32_t)) % BUFFER_CAPACITY;
//        readBytes(t, &lvl, sizeof(uint8_t));
//        t = (t + sizeof(uint8_t)) % BUFFER_CAPACITY;
//        if (len >= sizeof(msg)) len = sizeof(msg) - 1;
//        readBytes(t, reinterpret_cast<uint8_t*>(msg), len);
//        msg[len] = '\0';
//        t = (t + len) % BUFFER_CAPACITY;
//
//        dprintf(consoleFd, "[%010u][%s] %s\n", ts, levelToString(static_cast<Level>(lvl)), msg);
//        tail.store(t, std::memory_order_release);
//    }
//
//    static uint32_t getTimestamp()
//    {
//        struct timespec ts;
//        clock_gettime(CLOCK_REALTIME, &ts);
//        return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
//    }
//
//    size_t freeSpace(size_t h, size_t t) const
//    {
//        return (h >= t) ? BUFFER_CAPACITY - (h - t) : t - h;
//    }
//
//    void writeBytes(size_t pos, const uint8_t *data, size_t len)
//    {
//        for (size_t i = 0; i < len; i++) { buffer[pos] = data[i]; pos = (pos + 1) % BUFFER_CAPACITY; }
//    }
//
//    void readBytes(size_t pos, uint8_t *data, size_t len)
//    {
//        for (size_t i = 0; i < len; i++) { data[i] = buffer[pos]; pos = (pos + 1) % BUFFER_CAPACITY; }
//    }
//};
//
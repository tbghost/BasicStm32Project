/**
 ********************************************************************************
 * @file        Logger.cpp
 * 
 * @namespace   Utils
 * 
 * @brief       Utils, Logsystem implementation.
 * 
 * @author      toberg
 * 
 * @date        2024/12/07
********************************************************************************/

#include "Logger.hpp"

using namespace Utils;

void Logger::writeBytes(size_t index, const uint8_t* data, size_t len)
{
    size_t firstPart = std::min(len, BUFFER_CAPACITY - index);
    std::memcpy(&buffer[index], data, firstPart);
    if (len > firstPart)
    {
        std::memcpy(&buffer[0], data + firstPart, len - firstPart);
    }
}

explicit Logger::Logger(LogLevel suppLevel)
: head(0) 
, tail(0)
, running(true)
, mSuppLevel(suppLevel)
{
    consoleFd = open("/dev/console", O_RDWR);
    if (consoleFd < 0) consoleFd = 1; // fallback stdout
    pthread_create(&thread, nullptr, &Logger::ThreadEntry, this);
}


Logger::~Logger()
{
    running = false;
    pthread_join(thread, nullptr);
    if (consoleFd > 2) close(consoleFd);
}

void Logger::Log(LogLevel level, const char *msg)
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



    static void *threadEntry(void *arg)
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

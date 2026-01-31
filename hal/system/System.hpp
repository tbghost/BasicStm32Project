#ifndef SYSTEM_STM32H7XX_HPP
#define SYSTEM_STM32H7XX_HPP

#include <cstdint>
#include <cmath>

class System {
public:
    static uint32_t SystemCoreClock;
    static uint32_t SystemD2Clock;
    static const uint8_t D1CorePrescTable[16];

    static void SystemInit();
    static void SystemCoreClockUpdate();
};

#endif // SYSTEM_STM32H7XX_HPP
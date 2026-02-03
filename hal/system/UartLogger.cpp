/**
 ********************************************************************************
 * @file        UartLogger.cpp
 * 
 * @namespace   Utils
 * 
 * @brief       UART output adapter for Logger implementation
 * 
 * @author      toberg
 * 
 * @date        2026/01/31
 ********************************************************************************/

#include "UartLogger.hpp"
#include "stm32h7xx_hal.h"
#include <cstdio>
#include <cstdarg>
#include <cstring>

namespace Utils {

// UART handle for USART3 (ST-Link)
static UART_HandleTypeDef huart3;

void UartLogger::init()
{
    initUart();
    Logger::init();
    puts("\r\n========================================\r\n");
    Logger::info("Logger initialized - UART 115200 baud");
    puts("========================================\r\n");
}

void UartLogger::initUart()
{
    // Configure USART3 (ST-Link UART)
    // USART3: TX=PD8, RX=PD9
    
    // Enable USART3 clock
    __HAL_RCC_USART3_CLK_ENABLE();
    
    // Enable GPIO port D clock
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    // Configure GPIO pins for USART3
    GPIO_InitTypeDef GPIO_InitStruct;
    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;  // TX and RX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // Configure USART3
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLED;
    huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
    if (HAL_UART_Init(&huart3) != HAL_OK) {
        // Initialization error
        while (1);
    }
}

void UartLogger::putchar(char c)
{
    HAL_UART_Transmit(&huart3, (uint8_t*)&c, 1, 1000);
}

void UartLogger::puts(const char* str)
{
    if (!str) return;
    while (*str) {
        putchar(*str++);
    }
}

const char* UartLogger::levelToString(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO ";
        case LogLevel::WARN:     return "WARN ";
        case LogLevel::ERROR:    return "ERR  ";
        case LogLevel::CRITICAL: return "CRIT ";
        default:                 return "?    ";
    }
}

void UartLogger::processMessages()
{
    Message msg;
    while (Logger::getInstance().readMessage(msg)) {
        // Format: [LEVEL] message\r\n
        putchar('[');
        puts(levelToString(msg.level));
        puts("] ");
        puts(msg.text);
        puts("\r\n");
    }
    
    // Check for overflow
    if (Logger::getInstance().hasOverflowed()) {
        puts("[WARN ] Logger buffer overflow - messages were lost\r\n");
        Logger::getInstance().clearOverflowFlag();
    }
}

void UartLogger::printf(const char* format, ...)
{
    if (!format) return;
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    
    // Use vsnprintf to safely format the string
    int len = vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);
    
    if (len > 0) {
        buffer[len] = '\0';
        puts(buffer);
    }
}

}  // namespace Utils

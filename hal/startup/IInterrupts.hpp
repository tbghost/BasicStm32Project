/** 
 * @file        IInterrupts.hpp
 * 
 * @namespace   HAL
 * 
 * @brief       Interface of interrupt handling STM32H743xx.
 * 
 * @author      toberg
 * 
 * @date        2026/02/08
*/
#pragma once



namespace HAL {

/**
 * @brief IInterrupt_Handler interrupt vector table of the STM32H743xx
*/
class IInterrupts
{
    public:
    
        /// @brief Enumeration of the core exception vectors (first 16 entries in the vector table)
        enum class Exception_Vectors : uint8_t
        {
            ESTACK = 0U,            //!< Initial stack pointer.
            RESET_Handler,          //!< Reset handler (Entry point).
            NMI_Handler,            //!< Non maskable interrupt.
            HardFault_Handler,      //!< Hard fault interrupt.
            MemManage_Handler,      //!< Memory management interrupt.
            BusFault_Handler,       //!< Bus fault interrupt.
            UsageFault_Handler,     //!< Usage fault interrupt.
            RESERVED_Handler0,      //!< Reserved.
            RESERVED_Handler1,      //!< Reserved.
            RESERVED_Handler2,      //!< Reserved.
            RESERVED_Handler3,      //!< Reserved.
            SVC_Handler,            //!< SV call interrupt.
            DebugMon_Handler,       //!< Debug monitor interrupt.
            RESERVED_Handler4,      //!< Reserved.
            PendSV_Handler,         //!< Pend SV interrupt.
            SysTick_Handler = 15U   //!< System tick timer interrupt.
        };

        /// @brief Enumeration of the external interrupt vectors (starting from entry 16 in the vector table)          
        enum class External_Vectors : uint8_t
        {
            WWDG_IRQHandler = 0U,           //!< Window WatchDog                
            PVD_AVD_IRQHandler,             //!< PVD/AVD through EXTI Line detection
            TAMP_STAMP_IRQHandler,          //!< Tamper and TimeStamps through the EXTI line
            RTC_WKUP_IRQHandler,            //!< RTC Wakeup through the EXTI line 
            FLASH_IRQHandler,               //!< FLASH                          
            RCC_IRQHandler,                 //!< RCC                            
            EXTI0_IRQHandler,               //!< EXTI Line0                     
            EXTI1_IRQHandler,               //!< EXTI Line1                     
            EXTI2_IRQHandler,               //!< EXTI Line2                     
            EXTI3_IRQHandler,               //!< EXTI Line3                     
            EXTI4_IRQHandler,               //!< EXTI Line4                     
            DMA1_Stream0_IRQHandler,        //!< DMA1 Stream 0                  
            DMA1_Stream1_IRQHandler,        //!< DMA1 Stream 1                  
            DMA1_Stream2_IRQHandler,        //!< DMA1 Stream 2                  
            DMA1_Stream3_IRQHandler,        //!< DMA1 Stream 3                  
            DMA1_Stream4_IRQHandler,        //!< DMA1 Stream 4                  
            DMA1_Stream5_IRQHandler = 16U,  //!< DMA1 Stream 5                  
            DMA1_Stream6_IRQHandler,        //!< DMA1 Stream 6                  
            ADC_IRQHandler,                 //!< ADC1, ADC2 and ADC3s           
            FDCAN1_IT0_IRQHandler,          //!< FDCAN1 interrupt line 0        
            FDCAN2_IT0_IRQHandler,          //!< FDCAN2 interrupt line 0        
            FDCAN1_IT1_IRQHandler,          //!< FDCAN1 interrupt line 1        
            FDCAN2_IT1_IRQHandler,          //!< FDCAN2 interrupt line 1        
            EXTI9_5_IRQHandler,             //!< External Line[9:5]s            
            TIM1_BRK_IRQHandler,            //!< TIM1 Break interrupt           
            TIM1_UP_IRQHandler,             //!< TIM1 Update interrupt          
            TIM1_TRG_COM_IRQHandler,        //!< TIM1 Trigger and Commutation interrupt   
            TIM1_CC_IRQHandler,             //!< TIM1 Capture Compare           
            TIM2_IRQHandler,                //!< TIM2                           
            TIM3_IRQHandler,                //!< TIM3                           
            TIM4_IRQHandler,                //!< TIM4                           
            I2C1_EV_IRQHandler,             //!< I2C1 Event                     
            I2C1_ER_IRQHandler = 32U        //!< I2C1 Error                     
            I2C2_EV_IRQHandler,             //!< I2C2 Event                     
            I2C2_ER_IRQHandler,             //!< I2C2 Error                     
            SPI1_IRQHandler,                //!< SPI1                           
            SPI2_IRQHandler,                //!< SPI2                           
            USART1_IRQHandler,              //!< USART1                         
            USART2_IRQHandler,              //!< USART2                         
            USART3_IRQHandler,              //!< USART3                         
            EXTI15_10_IRQHandler,           //!< External Line[15:10]s          
            RTC_Alarm_IRQHandler,           //!< RTC Alarm (A and B) through EXTI 
            RESERVED0,                      //!< Reserved                       
            TIM8_BRK_TIM12_IRQHandler,      //!< TIM8 Break and TIM12          
            TIM8_UP_TIM13_IRQHandler,       //!< TIM8 Update and TIM13          
            TIM8_TRG_COM_TIM14_IRQHandler,  //!< TIM8 Trigger and Commutation and 
            TIM8_CC_IRQHandler,             //!< TIM8 Capture Compare           
            DMA1_Stream7_IRQHandler,        //!< DMA1 Stream7                   
            FMC_IRQHandler = 48U            //!< FMC                            
            SDMMC1_IRQHandler,              //!< SDMMC1                         
            TIM5_IRQHandler,                //!< TIM5                           
            SPI3_IRQHandler,                //!< SPI3                           
            UART4_IRQHandler,               //!< UART4                          
            UART5_IRQHandler,               //!< UART5                          
            TIM6_DAC_IRQHandler,            //!< TIM6 and DAC1&2 underrun errors 
            TIM7_IRQHandler,                //!< TIM7                         
            DMA2_Stream0_IRQHandler,        //!< DMA2 Stream 0                  
            DMA2_Stream1_IRQHandler,        //!< DMA2 Stream 1                  
            DMA2_Stream2_IRQHandler,        //!< DMA2 Stream 2                  
            DMA2_Stream3_IRQHandler,        //!< DMA2 Stream 3                  
            DMA2_Stream4_IRQHandler,        //!< DMA2 Stream 4                  
            ETH_IRQHandler,                 //!< Ethernet                       
            ETH_WKUP_IRQHandler,            //!< Ethernet Wakeup through EXTI line
            FDCAN_CAL_IRQHandler,           //!< FDCAN calibration unit interrupt
            RESERVED1 = 64U                 //!< Reserved                      
            RESERVED2,                      //!< Reserved                       
            RESERVED3,                      //!< Reserved                       
            RESERVED4,                      //!< Reserved                       
            DMA2_Stream5_IRQHandler,        //!< DMA2 Stream 5                
            DMA2_Stream6_IRQHandler,        //!< DMA2 Stream 6                  
            DMA2_Stream7_IRQHandler,        //!< DMA2 Stream 7                  
            USART6_IRQHandler,              //!< USART6                         
            I2C3_EV_IRQHandler,             //!< I2C3 event                     
            I2C3_ER_IRQHandler,             //!< I2C3 error                     
            OTG_HS_EP1_OUT_IRQHandler,      //!< USB OTG HS End Point 1 Out     
            OTG_HS_EP1_IN_IRQHandler,       //!< USB OTG HS End Point 1 In      
            OTG_HS_WKUP_IRQHandler,         //!< USB OTG HS Wakeup through EXTI 
            OTG_HS_IRQHandler,              //!< USB OTG HS                     
            DCMI_IRQHandler,                //!< DCMI                           
            RESERVED5,                      //!< Reserved                       
            RNG_IRQHandler = 80U,           //!< Rng                          
            FPU_IRQHandler,                 //!< FPU                          
            UART7_IRQHandler,               //!< UART7                          
            UART8_IRQHandler,               //!< UART8                        
            SPI4_IRQHandler,                //!< SPI4                         
            SPI5_IRQHandler,                //!< SPI5                         
            SPI6_IRQHandler,                //!< SPI6                         
            SAI1_IRQHandler,                //!< SAI1                         
            LTDC_IRQHandler,                //!< LTDC                         
            LTDC_ER_IRQHandler,             //!< LTDC error                   
            DMA2D_IRQHandler,               //!< DMA2D                        
            SAI2_IRQHandler,                //!< SAI2                         
            QUADSPI_IRQHandler,             //!< QUADSPI                      
            LPTIM1_IRQHandler,              //!< LPTIM1                       
            CEC_IRQHandler,                 //!< HDMI_CEC                     
            I2C4_EV_IRQHandler,             //!< I2C4 Event                   
            I2C4_ER_IRQHandler = 96U,       //!< I2C4 Error                   
            SPDIF_RX_IRQHandler,            //!< SPDIF_RX                      
            OTG_FS_EP1_OUT_IRQHandler,      //!< USB OTG FS End Point 1 Out     
            OTG_FS_EP1_IN_IRQHandler,       //!< USB OTG FS End Point 1 In      
            OTG_FS_WKUP_IRQHandler,         //!< USB OTG FS Wakeup through EXTI 
            OTG_FS_IRQHandler,              //!< USB OTG FS                   
            DMAMUX1_OVR_IRQHandler,         //!< DMAMUX1 Overrun interrupt      
            HRTIM1_Master_IRQHandler,       //!< HRTIM Master Timer global Interrupt
            HRTIM1_TIMA_IRQHandler,         //!< HRTIM Timer A global Interrupt 
            HRTIM1_TIMB_IRQHandler,         //!< HRTIM Timer B global Interrupt 
            HRTIM1_TIMC_IRQHandler,         //!< HRTIM Timer C global Interrupt 
            HRTIM1_TIMD_IRQHandler,         //!< HRTIM Timer D global Interrupt 
            HRTIM1_TIME_IRQHandler,         //!< HRTIM Timer E global Interrupt 
            HRTIM1_FLT_IRQHandler,          //!< HRTIM Fault global Interrupt   
            DFSDM1_FLT0_IRQHandler,         //!< DFSDM Filter0 Interrupt        
            DFSDM1_FLT1_IRQHandler,         //!< DFSDM Filter1 Interrupt        
            DFSDM1_FLT2_IRQHandler = 112U,  //!< DFSDM Filter2 Interrupt        
            DFSDM1_FLT3_IRQHandler,         //!< DFSDM Filter3 Interrupt        
            SAI3_IRQHandler,                //!< SAI3 global Interrupt          
            SWPMI1_IRQHandler,              //!< Serial Wire Interface 1 global in
            TIM15_IRQHandler,               //!< TIM15 global Interrupt         
            TIM16_IRQHandler,               //!< TIM16 global Interrupt         
            TIM17_IRQHandler,               //!< TIM17 global Interrupt         
            MDIOS_WKUP_IRQHandler,          //!< MDIOS Wakeup  Interrupt        
            MDIOS_IRQHandler,               //!< MDIOS global Interrupt         
            JPEG_IRQHandler,                //!< JPEG global Interrupt          
            MDMA_IRQHandler,                //!< MDMA global Interrupt          
            RESERVED6,                      //!< Reserved                       
            SDMMC2_IRQHandler,              //!< SDMMC2 global Interrupt        
            HSEM1_IRQHandler,               //!< HSEM1 global Interrupt         
            RESERVED7,                      //!< Reserved                       
            ADC3_IRQHandler,                //!< ADC3 global Interrupt          
            DMAMUX2_OVR_IRQHandler = 128U,  //!< DMAMUX Overrun interrupt       
            BDMA_Channel0_IRQHandler,       //!< BDMA Channel 0 global Interrupt 
            BDMA_Channel1_IRQHandler,       //!< BDMA Channel 1 global Interrupt 
            BDMA_Channel2_IRQHandler,       //!< BDMA Channel 2 global Interrupt 
            BDMA_Channel3_IRQHandler,       //!< BDMA Channel 3 global Interrupt 
            BDMA_Channel4_IRQHandler,       //!< BDMA Channel 4 global Interrupt 
            BDMA_Channel5_IRQHandler,       //!< BDMA Channel 5 global Interrupt 
            BDMA_Channel6_IRQHandler,       //!< BDMA Channel 6 global Interrupt 
            BDMA_Channel7_IRQHandler,       //!< BDMA Channel 7 global Interrupt 
            COMP1_IRQHandler,               //!< COMP1 global Interrupt         
            LPTIM2_IRQHandler,              //!< LP TIM2 global interrupt       
            LPTIM3_IRQHandler,              //!< LP TIM3 global interrupt       
            LPTIM4_IRQHandler,              //!< LP TIM4 global interrupt       
            LPTIM5_IRQHandler,              //!< LP TIM5 global interrupt       
            LPUART1_IRQHandler,             //!< LP UART1 interrupt             
            RESERVED8,                      //!< Reserved                        
            CRS_IRQHandler = 144U,          //!< Clock Recovery Global Interrupt 
            ECC_IRQHandler,                 //!< ECC diagnostic Global Interrupt 
            SAI4_IRQHandler,                //!< SAI4 global interrupt          
            RESERVED9,                      //!< Reserved                       
            RESERVED10,                     //!< Reserved                       
            WAKEUP_PIN_IRQHandler = 149U    //!< Interrupt for all 6 wake-up pins 
        };      // end of enum External_Vectors



};   // end of class

} // end namespace HAL


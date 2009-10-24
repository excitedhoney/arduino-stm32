//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

void ResetISR(void);
void NmiSR(void);
void FaultISR(void);
static void IntDefaultHandler(void);

void IntDefaultHandler(void)
{
    // Go into an infinite loop.
    while(1)
    {
    }
}

//*****************************************************************************
//
// Forward declaration of the interrupt handlers in this project.
//
//*****************************************************************************

extern void SysTickHandler(void);
void  USART1_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  USART2_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));

void  ADC_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  USART3_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  EXTI0_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  EXTI1_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  EXTI2_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  EXTI3_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  EXTI4_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  EXTI9_5_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  EXTI15_10_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMAChannel1_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMAChannel2_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMAChannel3_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMAChannel4_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMAChannel5_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMAChannel6_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMAChannel7_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));

void  I2C1_EV_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  I2C1_ER_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  I2C2_EV_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  I2C2_ER_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));

void  SPI1_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  SPI2_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));

void  TIM1_BRK_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM1_UP_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM1_TRG_CCUP_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM1_CC_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM2_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM3_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM4_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));

void  TIM8_BRK_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM8_UP_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM8_TRG_CCUP_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM8_CC_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  ADC3_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  SDIO_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM5_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  SPI3_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  UART4_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  UART5_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM6_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  TIM7_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMA2Channel1_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMA2Channel2_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMA2Channel3_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));
void  DMA2Channel4_5_IRQHandler (void) __attribute__ ((weak, alias ("IntDefaultHandler")));

__attribute__ ((section(".vectors")))
void (* const g_pfnVectors[])(void) =
{
    //  compile with gcc .... -DSTACKTOP=0x200027FC for 10kB RAM
    (void (*)(void)) STACKTOP,
    ResetISR,                               // The reset handler = program start
    NmiSR,                                  // The NMI handler
    FaultISR,                               // The hard fault handler
    IntDefaultHandler,                      //  MemManageException,
    IntDefaultHandler,                      //  BusFaultException,
    IntDefaultHandler,                      //  UsageFaultException,
    0, 0, 0, 0,                             /* Reserved */ 
    IntDefaultHandler,                      //  SVCHandler,
    IntDefaultHandler,                      //  DebugMonitor,
    0,                                      /* Reserved */
    IntDefaultHandler,                      //  PendSVC,
    SysTickHandler,                         //  SysTickHandler,
    IntDefaultHandler,                      //  (0) WWDG_IRQHandler,
    IntDefaultHandler,                      //  PVD_IRQHandler,
    IntDefaultHandler,                      //  TAMPER_IRQHandler,
    IntDefaultHandler,                      //  RTC_IRQHandler,
    IntDefaultHandler,                      //  FLASH_IRQHandler,
    IntDefaultHandler,                      //  RCC_IRQHandler,
    EXTI0_IRQHandler,                       //  EXTI0_IRQHandler,
    EXTI1_IRQHandler,                       //  EXTI1_IRQHandler,
    EXTI2_IRQHandler,                       //  EXTI2_IRQHandler,
    EXTI3_IRQHandler,                       //  EXTI3_IRQHandler,
    EXTI4_IRQHandler,                       //  (10) EXTI4_IRQHandler,
    DMAChannel1_IRQHandler,                 //  DMAChannel1_IRQHandler,
    DMAChannel2_IRQHandler,                 //  DMAChannel2_IRQHandler,
    DMAChannel3_IRQHandler,                 //  DMAChannel3_IRQHandler,
    DMAChannel4_IRQHandler,                 //  DMAChannel4_IRQHandler,
    DMAChannel5_IRQHandler,                 //  DMAChannel5_IRQHandler,
    DMAChannel6_IRQHandler,                 //  DMAChannel6_IRQHandler,
    DMAChannel7_IRQHandler,                 //  DMAChannel7_IRQHandler,
    ADC_IRQHandler,                         //  ADC_IRQHandler,
    IntDefaultHandler,                      //  USB_HP_CAN_TX_IRQHandler,
    IntDefaultHandler,                      //  (20) USB_LP_CAN_RX0_IRQHandler,
    IntDefaultHandler,                      //  CAN_RX1_IRQHandler,
    IntDefaultHandler,                      //  CAN_SCE_IRQHandler,
    EXTI9_5_IRQHandler,                     //  EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,                    //  TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler,                     //  TIM1_UP_IRQHandler,
    TIM1_TRG_CCUP_IRQHandler,               //  TIM1_TRG_CCUP_IRQHandler,
    TIM1_CC_IRQHandler,                     //  TIM1_CC_IRQHandler,
    TIM2_IRQHandler,                        //  TIM2_IRQHandler,
    TIM3_IRQHandler,                        //  TIM3_IRQHandler,
    TIM4_IRQHandler,                        //  (30) TIM4_IRQHandler,
    I2C1_EV_IRQHandler,                     //  I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,                     //  I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,                     //  I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,                     //  I2C2_ER_IRQHandler,
    SPI1_IRQHandler,                        //  SPI1_IRQHandler,
    SPI2_IRQHandler,                        //  SPI2_IRQHandler,
    USART1_IRQHandler,                      //  USART1_IRQHandler,
    USART2_IRQHandler,                      //  USART2_IRQHandler,
    USART3_IRQHandler,                      //  USART3_IRQHandler,
    EXTI15_10_IRQHandler,                      //  (40) EXTI15_10_IRQHandler,
    IntDefaultHandler,                      //  RTCAlarm_IRQHandler,
    IntDefaultHandler,                      //  USBWakeUp_IRQHandler,
    /* In part variants STM32F103xC, STM32F103xD, STM32F103xE */
    TIM8_BRK_IRQHandler,                    //  TIM8_BRK_IRQHandler,
    TIM8_UP_IRQHandler,                     //  TIM8_UP_IRQHandler,
    TIM8_TRG_CCUP_IRQHandler,               //  TIM8_TRG_CCUP_IRQHandler,
    TIM8_CC_IRQHandler,                     //  TIM8_CC_IRQHandler,
    ADC3_IRQHandler,                        //  ADC3_IRQHandler,
    IntDefaultHandler,                      //  FSMC_IRQHandler,
    SDIO_IRQHandler,                        //  SDIO_IRQHandler,
    TIM5_IRQHandler,                        //  (50) TIM5_IRQHandler,
    SPI3_IRQHandler,                        //  SPI3_IRQHandler,
    UART4_IRQHandler,                       //  UART4_IRQHandler,
    UART5_IRQHandler,                       //  UART5_IRQHandler,
    TIM6_IRQHandler,                        //  TIM6_IRQHandler,
    TIM7_IRQHandler,                        //  TIM7_IRQHandler,
    DMA2Channel1_IRQHandler,                //  DMA2Channel1_IRQHandler,
    DMA2Channel2_IRQHandler,                //  DMA2Channel2_IRQHandler,
    DMA2Channel3_IRQHandler,                //  DMA2Channel3_IRQHandler,
    DMA2Channel4_5_IRQHandler               //  DMA2Channel4_5_IRQHandler,
};




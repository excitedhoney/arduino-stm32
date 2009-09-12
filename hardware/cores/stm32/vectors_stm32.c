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
void IntDefaultHandler(void);

//*****************************************************************************
//
// Forward declaration of the interrupt handlers in this project.
//
//*****************************************************************************

extern void SysTickHandler(void);
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);

void  __attribute__ ((weak)) ADC_IRQHandler (void) {  };
void  __attribute__ ((weak)) USART3_IRQHandler (void) { };
void  __attribute__ ((weak)) EXTI0_IRQHandler (void) { };
void  __attribute__ ((weak)) EXTI1_IRQHandler (void) { };
void  __attribute__ ((weak)) EXTI2_IRQHandler (void) { };
void  __attribute__ ((weak)) EXTI3_IRQHandler (void) { };
void  __attribute__ ((weak)) EXTI4_IRQHandler (void) { };
void  __attribute__ ((weak)) I2C1_EV_IRQHandler (void) { };
void  __attribute__ ((weak)) I2C1_ER_IRQHandler (void) { };
void  __attribute__ ((weak)) I2C2_EV_IRQHandler (void) { };
void  __attribute__ ((weak)) I2C2_ER_IRQHandler (void) { };

void  __attribute__ ((weak)) SPI1_IRQHandler (void) { };
void  __attribute__ ((weak)) SPI2_IRQHandler (void) { };

void  __attribute__ ((weak)) TIM2_IRQHandler (void) { };
void  __attribute__ ((weak)) TIM3_IRQHandler (void) { };
void  __attribute__ ((weak)) TIM4_IRQHandler (void) { };


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
    IntDefaultHandler,                      //  DMAChannel1_IRQHandler,
    IntDefaultHandler,                      //  DMAChannel2_IRQHandler,
    IntDefaultHandler,                      //  DMAChannel3_IRQHandler,
    IntDefaultHandler,                      //  DMAChannel4_IRQHandler,
    IntDefaultHandler,                      //  DMAChannel5_IRQHandler,
    IntDefaultHandler,                      //  DMAChannel6_IRQHandler,
    IntDefaultHandler,                      //  DMAChannel7_IRQHandler,
    ADC_IRQHandler,                         //  ADC_IRQHandler,
    IntDefaultHandler,                      //  USB_HP_CAN_TX_IRQHandler,
    IntDefaultHandler,                      //  (20) USB_LP_CAN_RX0_IRQHandler,
    IntDefaultHandler,                      //  CAN_RX1_IRQHandler,
    IntDefaultHandler,                      //  CAN_SCE_IRQHandler,
    IntDefaultHandler,                      //  EXTI9_5_IRQHandler,
    IntDefaultHandler,                      //  TIM1_BRK_IRQHandler,
    IntDefaultHandler,                      //  TIM1_UP_IRQHandler,
    IntDefaultHandler,                      //  TIM1_TRG_CCUP_IRQHandler,
    IntDefaultHandler,                      //  TIM1_CC_IRQHandler,
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
    IntDefaultHandler,                      //  (40) EXTI15_10_IRQHandler,
    IntDefaultHandler,                      //  RTCAlarm_IRQHandler,
    IntDefaultHandler,                      //  USBWakeUp_IRQHandler,
    /* In part variants STM32F103xC, STM32F103xD, STM32F103xE */
    IntDefaultHandler,                      //  TIM8_BRK_IRQHandler,
    IntDefaultHandler,                      //  TIM8_UP_IRQHandler,
    IntDefaultHandler,                      //  TIM8_TRG_CCUP_IRQHandler,
    IntDefaultHandler,                      //  TIM8_CC_IRQHandler,
    IntDefaultHandler                       //  ADC3_IRQHandler,
};




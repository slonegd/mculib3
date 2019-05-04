#pragma once
#include "interrupt.h"
#include "periph_usart.h"
#include "periph_dma.h"
#include "periph_spi.h"
#include "periph_exti.h"


#if defined (STM32F0)
   extern "C" void DMA1_Channel1_IRQHandler() { interrupt_DMA1_channel1.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_1);}
   extern "C" void DMA1_Channel2_3_IRQHandler()     { 
      interrupt_DMA1_channel2.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_2); 
      interrupt_DMA1_channel3.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_3); 
   }
   extern "C" void DMA1_Channel4_5_IRQHandler()     { while(1) {} }

   extern "C" void TIM1_BRK_TIM9_IRQHandler      () { while(1) {} }
   extern "C" void TIM1_CC_IRQHandler            () { while(1) {} }
   extern "C" void TIM3_IRQHandler               () { while(1) {} }
   extern "C" void TIM14_IRQHandler              () { while(1) {} }
   extern "C" void TIM16_IRQHandler              () { while(1) {} }
   extern "C" void TIM17_IRQHandler              () { while(1) {} }

   extern "C" void USART1_IRQHandler() { 
      interrupt_usart1.interrupt(); 
      mcu::make_reference<mcu::Periph::USART1>().clear_interrupt_flags();} 


#elif defined (STM32F1)
	// список прерываний, которые не описаны
   extern "C" void WWDG_IRQHandler            () { while(1) {} }
   extern "C" void PVD_IRQHandler             () { while(1) {} }
   extern "C" void TAMPER_IRQHandler          () { while(1) {} }
   extern "C" void RTC_IRQHandler             () { while(1) {} }
   extern "C" void FLASH_IRQHandler           () { while(1) {} }
   extern "C" void RCC_IRQHandler             () { while(1) {} }
   extern "C" void EXTI0_IRQHandler           () { interrupt_EXTI0.interrupt(); REF(EXTI).clear_interrupt_flags<0>(); }
   extern "C" void EXTI1_IRQHandler           () { interrupt_EXTI1.interrupt(); REF(EXTI).clear_interrupt_flags<1>(); }
   extern "C" void EXTI2_IRQHandler           () { interrupt_EXTI2.interrupt(); REF(EXTI).clear_interrupt_flags<2>(); }
   extern "C" void EXTI3_IRQHandler           () { interrupt_EXTI3.interrupt(); REF(EXTI).clear_interrupt_flags<3>(); }
   extern "C" void EXTI4_IRQHandler           () { interrupt_EXTI4.interrupt(); REF(EXTI).clear_interrupt_flags<4>(); }
   extern "C" void ADC1_2_IRQHandler          () { while(1) {} }
   extern "C" void USB_HP_CAN1_TX_IRQHandler  () { while(1) {} }
   extern "C" void USB_LP_CAN1_RX0_IRQHandler () { while(1) {} }
   extern "C" void CAN1_RX1_IRQHandler        () { while(1) {} }
   extern "C" void CAN1_SCE_IRQHandler        () { while(1) {} }
   extern "C" void EXTI9_5_IRQHandler         () { interrupt_EXTI9_5.interrupt(); REF(EXTI).clear_interrupt_flags<5,9>(); }
   extern "C" void TIM1_BRK_IRQHandler        () { while(1) {} }
   extern "C" void TIM1_UP_IRQHandler         () { while(1) {} }
   extern "C" void TIM1_TRG_COM_IRQHandler    () { while(1) {} }
   extern "C" void TIM1_CC_IRQHandler         () { while(1) {} }
   extern "C" void TIM2_IRQHandler            () { while(1) {} }
   extern "C" void TIM3_IRQHandler            () { while(1) {} }
   extern "C" void TIM4_IRQHandler            () { while(1) {} }
   extern "C" void I2C1_EV_IRQHandler         () { while(1) {} }
   extern "C" void I2C1_ER_IRQHandler         () { while(1) {} }
   extern "C" void I2C2_EV_IRQHandler         () { while(1) {} }
   extern "C" void I2C2_ER_IRQHandler         () { while(1) {} }
   extern "C" void SPI1_IRQHandler            () { while(1) {} }
   extern "C" void SPI2_IRQHandler            () { while(1) {} }
   extern "C" void EXTI15_10_IRQHandler       () { interrupt_EXTI15_10.interrupt(); REF(EXTI).clear_interrupt_flags<10,15>(); }
   extern "C" void RTC_Alarm_IRQHandler       () { while(1) {} }
   extern "C" void USBWakeUp_IRQHandler       () { while(1) {} }

   extern "C" void USART1_IRQHandler          () { interrupt_usart1.interrupt(); mcu::make_reference<mcu::Periph::USART1>().clear_interrupt_flags();}
   extern "C" void USART2_IRQHandler          () { interrupt_usart2.interrupt(); mcu::make_reference<mcu::Periph::USART2>().clear_interrupt_flags();}
   extern "C" void USART3_IRQHandler          () { interrupt_usart3.interrupt(); mcu::make_reference<mcu::Periph::USART3>().clear_interrupt_flags();}

   extern "C" void DMA1_Channel1_IRQHandler   () { interrupt_DMA1_channel1.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_1);}
   extern "C" void DMA1_Channel2_IRQHandler   () { interrupt_DMA1_channel2.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_2);}
   extern "C" void DMA1_Channel3_IRQHandler   () { interrupt_DMA1_channel3.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_3);}
   extern "C" void DMA1_Channel4_IRQHandler   () { interrupt_DMA1_channel4.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_4);}
   extern "C" void DMA1_Channel5_IRQHandler   () { interrupt_DMA1_channel5.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_5);}
   extern "C" void DMA1_Channel6_IRQHandler   () { interrupt_DMA1_channel6.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_6);}
   extern "C" void DMA1_Channel7_IRQHandler   () { interrupt_DMA1_channel7.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_7);}

#elif defined (STM32F4)
   extern "C" void WWDG_IRQHandler               () { while(1) {} }
   extern "C" void PVD_IRQHandler                () { while(1) {} }
   extern "C" void TAMP_STAMP_IRQHandler         () { while(1) {} }
   extern "C" void RTC_WKUP_IRQHandler           () { while(1) {} }
   extern "C" void FLASH_IRQHandler              () { while(1) {} }
   extern "C" void RCC_IRQHandler                () { while(1) {} }
   extern "C" void EXTI0_IRQHandler              () { while(1) {} }
   extern "C" void EXTI1_IRQHandler              () { while(1) {} }
   extern "C" void EXTI2_IRQHandler              () { while(1) {} }
   extern "C" void EXTI3_IRQHandler              () { while(1) {} }
   extern "C" void EXTI4_IRQHandler              () { while(1) {} }
   extern "C" void ADC_IRQHandler                () { while(1) {} }
   extern "C" void CAN1_TX_IRQHandler            () { while(1) {} }
   extern "C" void CAN1_RX0_IRQHandler           () { while(1) {} }
   extern "C" void CAN1_RX1_IRQHandler           () { while(1) {} }
   extern "C" void CAN1_SCE_IRQHandler           () { while(1) {} }
   extern "C" void EXTI9_5_IRQHandler            () { while(1) {} }
   extern "C" void I2C1_EV_IRQHandler            () { while(1) {} }
   extern "C" void I2C1_ER_IRQHandler            () { while(1) {} }
   extern "C" void I2C2_EV_IRQHandler            () { while(1) {} }
   extern "C" void I2C2_ER_IRQHandler            () { while(1) {} }
   extern "C" void EXTI15_10_IRQHandler          () { while(1) {} }
   extern "C" void RTC_Alarm_IRQHandler          () { while(1) {} }
   extern "C" void OTG_FS_WKUP_IRQHandler        () { while(1) {} }
   extern "C" void FSMC_IRQHandler               () { while(1) {} }
   extern "C" void SDIO_IRQHandler               () { while(1) {} }
   extern "C" void UART4_IRQHandler              () { while(1) {} }
   extern "C" void UART5_IRQHandler              () { while(1) {} }
   extern "C" void CAN2_TX_IRQHandler            () { while(1) {} }
   extern "C" void CAN2_RX0_IRQHandler           () { while(1) {} }
   extern "C" void CAN2_RX1_IRQHandler           () { while(1) {} }
   extern "C" void CAN2_SCE_IRQHandler           () { while(1) {} }
   extern "C" void OTG_FS_IRQHandler             () { while(1) {} }
   extern "C" void USART6_IRQHandler             () { while(1) {} }
   extern "C" void I2C3_EV_IRQHandler            () { while(1) {} }
   extern "C" void I2C3_ER_IRQHandler            () { while(1) {} }
   extern "C" void OTG_HS_EP1_OUT_IRQHandler     () { while(1) {} }
   extern "C" void OTG_HS_EP1_IN_IRQHandler      () { while(1) {} }
   extern "C" void OTG_HS_WKUP_IRQHandler        () { while(1) {} }
   extern "C" void OTG_HS_IRQHandler             () { while(1) {} }
   extern "C" void HASH_RNG_IRQHandler           () { while(1) {} }
   extern "C" void FPU_IRQHandler                () { while(1) {} }

   extern "C" void TIM1_BRK_TIM9_IRQHandler      () { while(1) {} }
   extern "C" void TIM1_UP_TIM10_IRQHandler      () { while(1) {} }
   extern "C" void TIM1_TRG_COM_TIM11_IRQHandler () { while(1) {} }
   extern "C" void TIM1_CC_IRQHandler            () { while(1) {} }
   extern "C" void TIM2_IRQHandler               () { while(1) {} }
   extern "C" void TIM3_IRQHandler               () { while(1) {} }
   extern "C" void TIM4_IRQHandler               () { while(1) {} }
   extern "C" void TIM5_IRQHandler               () { while(1) {} }
   extern "C" void TIM6_DAC_IRQHandler           () { while(1) {} }
   extern "C" void TIM7_IRQHandler               () { while(1) {} }
   extern "C" void TIM8_BRK_TIM12_IRQHandler     () { while(1) {} }
   extern "C" void TIM8_UP_TIM13_IRQHandler      () { while(1) {} }
   extern "C" void TIM8_TRG_COM_TIM14_IRQHandler () { while(1) {} }
   extern "C" void TIM8_CC_IRQHandler            () { while(1) {} }

   
   extern "C" void USART1_IRQHandler()       { interrupt_usart1.interrupt(); mcu::make_reference<mcu::Periph::USART1>().clear_interrupt_flags();} 
   extern "C" void USART2_IRQHandler()       { interrupt_usart2.interrupt(); mcu::make_reference<mcu::Periph::USART2>().clear_interrupt_flags();}
   extern "C" void USART3_IRQHandler()       { interrupt_usart3.interrupt(); mcu::make_reference<mcu::Periph::USART3>().clear_interrupt_flags();}

   extern "C" void DMA1_Stream0_IRQHandler() { interrupt_DMA1_channel0.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_0); }
   extern "C" void DMA1_Stream1_IRQHandler() { interrupt_DMA1_channel1.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_1); }
   extern "C" void DMA1_Stream2_IRQHandler() { interrupt_DMA1_channel2.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_2); }
   extern "C" void DMA1_Stream3_IRQHandler() { interrupt_DMA1_channel3.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_3); }
   extern "C" void DMA1_Stream4_IRQHandler() { interrupt_DMA1_channel4.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_5); }
   extern "C" void DMA1_Stream5_IRQHandler() { interrupt_DMA1_channel5.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_5); }
   extern "C" void DMA1_Stream6_IRQHandler() { interrupt_DMA1_channel6.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_6); }
   extern "C" void DMA1_Stream7_IRQHandler() { interrupt_DMA1_channel7.interrupt(); mcu::make_reference<mcu::Periph::DMA1>().clear_interrupt_flags(mcu::DMA::Channel::_7); }
   extern "C" void DMA2_Stream0_IRQHandler() { interrupt_DMA2_channel0.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_0); }
   extern "C" void DMA2_Stream1_IRQHandler() { interrupt_DMA2_channel1.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_1); }
   extern "C" void DMA2_Stream2_IRQHandler() { interrupt_DMA2_channel2.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_2); }
   extern "C" void DMA2_Stream3_IRQHandler() { interrupt_DMA2_channel3.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_3); }
   extern "C" void DMA2_Stream4_IRQHandler() { interrupt_DMA2_channel4.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_4); }
   extern "C" void DMA2_Stream5_IRQHandler() { interrupt_DMA2_channel5.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_5); }
   extern "C" void DMA2_Stream6_IRQHandler() { interrupt_DMA2_channel6.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_6); }
   extern "C" void DMA2_Stream7_IRQHandler() { interrupt_DMA2_channel7.interrupt(); mcu::make_reference<mcu::Periph::DMA2>().clear_interrupt_flags(mcu::DMA::Channel::_7); }

   extern "C" void SPI1_IRQHandler() { interrupt_spi1.interrupt(); }
   extern "C" void SPI2_IRQHandler() { interrupt_spi2.interrupt(); }
   extern "C" void SPI3_IRQHandler() { interrupt_spi3.interrupt(); }
   #endif

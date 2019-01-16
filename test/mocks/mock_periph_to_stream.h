#pragma once

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::Periph v)
{
   return
      #define HELPER(p) v == mcu::Periph::p ? s << #p :
      HELPER (GPIOA)
      HELPER (GPIOB)
      HELPER (GPIOC)
      HELPER (GPIOD)
   #if defined(STM32F0)   
      HELPER (GPIOF)
   #elif defined(STM32F1)
      HELPER (GPIOE)
      HELPER (AFIO)
   #endif
   #if defined(STM32F4) or defined(STM32F7)
      HELPER (GPIOE)
      HELPER (GPIOG)
      HELPER (GPIOH)
      HELPER (GPIOI)
   #endif
      HELPER (USART1)
   #if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
      HELPER (USART2)
      HELPER (USART3)
   #endif
   #if defined(STM32F4) or defined(STM32F7)
      HELPER (USART4)
      HELPER (USART5)
      HELPER (USART6)
   #endif
   #if defined(STM32F7)
      HELPER (USART7)
      HELPER (USART8)
   #endif
      HELPER (DMA1)
      v == mcu::Periph::DMA1_stream1 ? s << "DMA1" :
      v == mcu::Periph::DMA1_stream2 ? s << "DMA1" :
      v == mcu::Periph::DMA1_stream3 ? s << "DMA1" :
      v == mcu::Periph::DMA1_stream4 ? s << "DMA1" :
      v == mcu::Periph::DMA1_stream5 ? s << "DMA1" :
   #if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
      v == mcu::Periph::DMA1_stream6 ? s << "DMA1" :
      v == mcu::Periph::DMA1_stream7 ? s << "DMA1" :
   #endif
   #if defined(STM32F4) or defined(STM32F7)
      v == mcu::Periph::DMA1_stream0 ? s << "DMA1" :
   #endif
      s << "допиши вывод в поток переферии";
      #undef HELPER
}

} // namespace mock {
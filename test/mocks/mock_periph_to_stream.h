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
   #if defined(STM32F4) or defined(STM32F7)
      HELPER (USART2)
      HELPER (USART3)
      HELPER (USART4)
      HELPER (USART5)
      HELPER (USART6)
   #endif
   #if defined(STM32F7)
      HELPER (USART7)
      HELPER (USART8)
   #endif
      s;
      #undef HELPER
}

} // namespace mock {
#pragma once

#define USE_PERIPH_MOCK
#include "periph_rcc.h"
#include <iostream>

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::RCC::AHBprescaler v)
{
   return
      v == mcu::RCC::AHBprescaler::AHBnotdiv ? s << "1"   :
      v == mcu::RCC::AHBprescaler::AHBdiv2   ? s << "2"   : 
      v == mcu::RCC::AHBprescaler::AHBdiv4   ? s << "4"   :
      v == mcu::RCC::AHBprescaler::AHBdiv8   ? s << "8"   :
      v == mcu::RCC::AHBprescaler::AHBdiv16  ? s << "16"  :
      v == mcu::RCC::AHBprescaler::AHBdiv64  ? s << "64"  :
      v == mcu::RCC::AHBprescaler::AHBdiv128 ? s << "128" :
      v == mcu::RCC::AHBprescaler::AHBdiv256 ? s << "256" :
      v == mcu::RCC::AHBprescaler::AHBdiv512 ? s << "512" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::RCC::APBprescaler v)
{
   return
      v == mcu::RCC::APBprescaler::APBnotdiv ? s << "1"   :
      v == mcu::RCC::APBprescaler::APBdiv2   ? s << "2"   : 
      v == mcu::RCC::APBprescaler::APBdiv4   ? s << "4"   :
      v == mcu::RCC::APBprescaler::APBdiv8   ? s << "8"   :
      v == mcu::RCC::APBprescaler::APBdiv16  ? s << "16"  : s;
}

std::ostream& operator<< (std::ostream& s, mcu::RCC::SystemClock v)
{
   return
      v == mcu::RCC::SystemClock::CS_HSE ? s << "внешнего источника"    :
      v == mcu::RCC::SystemClock::CS_HSI ? s << "внутреннего источника" : 
      v == mcu::RCC::SystemClock::CS_PLL ? s << "PLL"                   : s;
}

std::ostream& operator<< (std::ostream& s, mcu::RCC::PLLPdiv v)
{
   return
      v == mcu::RCC::PLLPdiv::PLLdiv2 ? s << "2" :
      v == mcu::RCC::PLLPdiv::PLLdiv4 ? s << "4" :
      v == mcu::RCC::PLLPdiv::PLLdiv6 ? s << "6" :
      v == mcu::RCC::PLLPdiv::PLLdiv8 ? s << "8" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::RCC::PLLsource v)
{
   return
      v == mcu::RCC::PLLsource::HSE     ? s << "внешний" :
      v == mcu::RCC::PLLsource::HSIdiv2 ? s << "внутренний с делителем 2" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::Periph v)
{
   return
      #define HELPER(p) v == mcu::Periph::p ? s << #p :
      HELPER (GPIOA)
      HELPER (GPIOB)
      HELPER (GPIOC)
      HELPER (GPIOD)
      HELPER (GPIOE)
      HELPER (GPIOF)
      HELPER (GPIOG)
      HELPER (GPIOH)
      HELPER (GPIOI)

      HELPER (USART1)
      HELPER (USART2)
      HELPER (USART3)
      HELPER (USART4)
      HELPER (USART5)
      HELPER (USART6)
      HELPER (USART7)
      HELPER (USART8)
      s;
      #undef HELPER
}

class RCC : public mcu::RCC
{
   std::ostream* process {nullptr};
   RCC() = default;
public:
   static RCC& make()
   {
      static RCC rcc;
      return rcc;
   }
   void set_stream (std::ostream& s) { process = &s; }

   RCC& set (AHBprescaler v) {
      if (process) *process << "установка делителя шины AHB " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

   RCC& set_APB1  (APBprescaler v) {
      if (process) *process << "установка делителя шины APB1 " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_APB1(v);
      return *this;
   }

   RCC& set_APB2  (APBprescaler v) {
      if (process) *process << "установка делителя шины APB2 " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_APB1(v);
      return *this;
   }

   RCC& set (SystemClock v) {
      if (process) *process << "установка источника тактирования системной шины от " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

   RCC& set (PLLPdiv v) {
      if (process) *process << "установка делителя шины PLL " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

   RCC& set (PLLsource v) {
      if (process) *process << "установка источника PLL " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

   template<int v> RCC& set_PLLM() {
      if (process) *process << "установка M " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_PLLM<v>();
      return *this;
   }

   template<int v> RCC& set_PLLN() {
      if (process) *process << "установка N " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_PLLN<v>();
      return *this;
   }

   template<int v> RCC& set_PLLQ() {
      if (process) *process << "установка Q " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_PLLQ<v>();
      return *this;
   }

   RCC& on_HSE() {
      if (process) *process << "включение тактирования от внешнего источника" << std::endl;
      static_cast<mcu::RCC*>(this)->on_HSE();
      return *this;
   }

   RCC& wait_HSE_ready() {
      if (this->like_CMSIS().CR | RCC_CR_HSEON_Msk) this->like_CMSIS().CR |= RCC_CR_HSERDY;
      static_cast<mcu::RCC*>(this)->wait_HSE_ready();
      if (process) *process << "ожидание готовности тактирования от внешнего источника" << std::endl;
      return *this;
   }

   RCC& on_PLL() {
      if (process) *process << "включение PLL" << std::endl;
      static_cast<mcu::RCC*>(this)->on_PLL();
      return *this;
   }

   RCC& wait_PLL_ready() {
      if (this->like_CMSIS().CR | RCC_CR_PLLON_Msk) this->like_CMSIS().CR |= RCC_CR_PLLRDY;
      static_cast<mcu::RCC*>(this)->wait_PLL_ready();
      if (process) *process << "ожидание готовности PLL" << std::endl;
      return *this;
   }

   template<mcu::Periph v> RCC& clock_enable() {
      if (process) *process << "включение тактирования " << v << std::endl;
      static_cast<mcu::RCC*>(this)->clock_enable<v>();
      return *this;
   }
};

} // namespace mock {

#if defined(USE_PERIPH_MOCK)
namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::RCC, mock::RCC&> make_reference() { return mock::RCC::make(); }
}
#endif
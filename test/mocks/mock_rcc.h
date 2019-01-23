#pragma once

#define USE_MOCK_RCC
#include "periph_rcc.h"
#include "mock_periph_to_stream.h"
#include "process.h"

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

#if defined(STM32F4) or defined(STM32F7)
std::ostream& operator<< (std::ostream& s, mcu::RCC::PLLPdiv v)
{
   return
      v == mcu::RCC::PLLPdiv::_2 ? s << "2" :
      v == mcu::RCC::PLLPdiv::_4 ? s << "4" :
      v == mcu::RCC::PLLPdiv::_6 ? s << "6" :
      v == mcu::RCC::PLLPdiv::_8 ? s << "8" : s;
}
#endif

std::ostream& operator<< (std::ostream& s, mcu::RCC::PLLsource v)
{
   return
      v == mcu::RCC::PLLsource::HSE     ? s << "внешний" :
      v == mcu::RCC::PLLsource::HSIdiv2 ? s << "внутренний с делителем 2" : s;
}

class RCC : public mcu::RCC
{
   Process& process { Process::make() };
   RCC() = default;
public:
   static RCC& make()
   {
      static RCC rcc;
      return rcc;
   }

   RCC& set (AHBprescaler v) {
      process << "установка делителя шины AHB " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

#if defined(STM32F4) or defined(STM32F7)
   RCC& set_APB1  (APBprescaler v) {
      process << "установка делителя шины APB1 " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_APB1(v);
      return *this;
   }

   RCC& set_APB2  (APBprescaler v) {
      process << "установка делителя шины APB2 " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_APB2(v);
      return *this;
   }

   RCC& set (PLLPdiv v) {
      process << "установка делителя шины PLL " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

   template<int v> RCC& set_PLLM() {
      process << "установка M " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_PLLM<v>();
      return *this;
   }

   template<int v> RCC& set_PLLN() {
      process << "установка N " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_PLLN<v>();
      return *this;
   }

   template<int v> RCC& set_PLLQ() {
      process << "установка Q " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set_PLLQ<v>();
      return *this;
   }
#elif defined(STM32F0)
   RCC& set (APBprescaler  v) {
      process << "установка делителя шины APB " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

   RCC& set (PLLmultiplier v) {
      process << "установка множителя PLL " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }
#endif

   RCC& set (SystemClock v) {
      process << "установка источника тактирования системной шины от " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }



   RCC& set (PLLsource v) {
      process << "установка источника PLL " << v << std::endl;
      static_cast<mcu::RCC*>(this)->set(v);
      return *this;
   }

   RCC& on_HSE() {
      process << "включение тактирования от внешнего источника" << std::endl;
      static_cast<mcu::RCC*>(this)->on_HSE();
      return *this;
   }

   RCC& wait_HSE_ready() {
      if (this->like_CMSIS().CR | RCC_CR_HSEON_Msk) this->like_CMSIS().CR |= RCC_CR_HSERDY;
      static_cast<mcu::RCC*>(this)->wait_HSE_ready();
      process << "ожидание готовности тактирования от внешнего источника" << std::endl;
      return *this;
   }

   RCC& on_PLL() {
      process << "включение PLL" << std::endl;
      static_cast<mcu::RCC*>(this)->on_PLL();
      return *this;
   }

   RCC& wait_PLL_ready() {
      if (this->like_CMSIS().CR | RCC_CR_PLLON_Msk) this->like_CMSIS().CR |= RCC_CR_PLLRDY;
      static_cast<mcu::RCC*>(this)->wait_PLL_ready();
      process << "ожидание готовности PLL" << std::endl;
      return *this;
   }

   template<mcu::Periph v> RCC& clock_enable() {
      process << "включение тактирования " << v << std::endl;
      static_cast<mcu::RCC*>(this)->clock_enable<v>();
      return *this;
   }
};

} // namespace mock {

namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::RCC, mock::RCC&> make_reference() { return mock::RCC::make(); }
}


namespace mock {
   auto& rcc = REF(RCC);
}
#pragma once

#include "rcc.h"
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
};

} // namespace mock {

namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::RCC, mock::RCC&> make_reference() { return mock::RCC::make(); }
}
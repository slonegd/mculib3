#pragma once

#define USE_MOCK_AFIO
#include "mock_rcc.h"
#include "periph_afio.h"
#include "mock_periph_to_stream.h"
#include "process.h"

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::AFIO::SWJ v)
{
   return
      v == mcu::AFIO::SWJ::Full_SWJ        ? s << "включение JTAG и SW полностью"  :
      v == mcu::AFIO::SWJ::Full_SWJ_NJTRST ? s << "включение JTAG и SW без NJTRST" :
      v == mcu::AFIO::SWJ::JTAG_off_SW_off ? s << "выключение JTAG и SW"           :
      v == mcu::AFIO::SWJ::JTAG_off_SW_on  ? s << "выключение JTAG"                : s;
}

class AFIO : public mcu::AFIO
{
   Process& process { Process::make() };
   AFIO() = default;
public:
   static AFIO& make()
   {
      static AFIO afio;
      return afio;
   }

   AFIO& set_JTAG (SWJ v)
   {
      process << v << std::endl;
      static_cast<mcu::AFIO*>(this)->set_JTAG(v);
      return *this;
   }

   template<mcu::Periph p> AFIO& remap()
   {
      process << "ремапинг " << p << std::endl;
      static_cast<mcu::AFIO*>(this)->remap<p>();
      return *this;
   }

   template<mcu::Periph p, mcu::AFIO::Remap r> AFIO& remap()
   {
      if constexpr (r == mcu::AFIO::Remap::Full)
         process << "полный ремапинг "    << p << std::endl;
      if constexpr (r == mcu::AFIO::Remap::Partial_1 or r == mcu::AFIO::Remap::Partial_2)
         process << "частичный ремапинг " << p << std::endl;
      if constexpr (r == mcu::AFIO::Remap::No)
         process << "отключение ремапа "  << p << std::endl;

      static_cast<mcu::AFIO*>(this)->remap<p,r>();
      return *this;
   }
};

} // namespace mock {

namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::AFIO, mock::AFIO&> make_reference() { return mock::AFIO::make(); }
}

namespace mock {
   auto& afio = REF(AFIO);
}
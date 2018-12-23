#pragma once

#define USE_MOCK_AFIO
#include "periph_afio.h"
#include "mock_periph_to_stream.h"
#include <iostream>

namespace mock {

class AFIO : public mcu::AFIO
{
   std::ostream* process {nullptr};
   AFIO() = default;
public:
   static AFIO& make()
   {
      static AFIO afio;
      return afio;
   }
   void set_stream (std::ostream& s) { process = &s; }

   template<mcu::Periph p> AFIO& remap()
   {
      if (process) *process << "ремапинг " << p << std::endl;
      static_cast<mcu::AFIO*>(this)->remap<p>();
      return *this;
   }

   template<mcu::Periph p, mcu::AFIO::Remap r> AFIO& remap()
   {
      if (process) {
         if constexpr (r == mcu::AFIO::Remap::Full)
            *process << "полный ремапинг "    << p << std::endl;
         if constexpr (r == mcu::AFIO::Remap::Partial_1 or r == mcu::AFIO::Remap::Partial_2)
            *process << "частичный ремапинг " << p << std::endl;
         if constexpr (r == mcu::AFIO::Remap::No)
            *process << "отключение ремапа "  << p << std::endl;
      }
      static_cast<mcu::AFIO*>(this)->remap<p,r>();
      return *this;
   }
};

} // namespace mock {

namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::AFIO, mock::AFIO&> make_reference() { return mock::AFIO::make(); }
}
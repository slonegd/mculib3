#pragma once

#define USE_MOCKS
#include "periph_flash.h"
#include <iostream>

namespace mock {

class FLASH : public mcu::FLASH
{
   std::ostream* process {nullptr};
   FLASH() = default;
public:
   static FLASH& make()
   {
      static FLASH flash;
      return flash;
   }
   void set_stream (std::ostream& s) { process = &s; }

   FLASH& set (Latency v) {
      if (process) *process << "установка задержки чтения памяти " << static_cast<size_t>(v) << std::endl;
      static_cast<mcu::FLASH*>(this)->set(v);
      return *this;
   }

};

} // namespace mock {

#if defined(USE_MOCKS)
namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::FLASH, mock::FLASH&> make_reference() { return mock::FLASH::make(); }
}
#endif
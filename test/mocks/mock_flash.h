#pragma once

#define USE_PERIPH_MOCK
#include "periph_flash.h"
#include "mock_memory.h"
#include <iostream>

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::FLASH::ProgSize v)
{
   return
      v == mcu::FLASH::ProgSize::x8  ? s << "8 бит"   :
      v == mcu::FLASH::ProgSize::x16 ? s << "16 бит"  :
      v == mcu::FLASH::ProgSize::x32 ? s << "32 бита" :
      v == mcu::FLASH::ProgSize::x64 ? s << "64 бита" : s;
}

class FLASH : public mcu::FLASH
{
   std::ostream* process {nullptr};
   FLASH() { this->lock(); }
   size_t eop_count {0};
   void (*erase) (Sector) {nullptr};
public:
   static FLASH& make()
   {
      static FLASH flash;
      return flash;
   }
   void set_stream (std::ostream& s) { process = &s; }
   void set_erase_function (void (&v) (Sector)) { erase = &v; }

   FLASH& set (Latency v) {
      if (process) *process << "установка задержки чтения памяти " << static_cast<size_t>(v) << std::endl;
      static_cast<mcu::FLASH*>(this)->set(v);
      return *this;
   }

   FLASH& lock() {
      if (process) *process << "рблокировка памяти для записи" << std::endl;
      static_cast<mcu::FLASH*>(this)->lock();
      return *this;
   }

   FLASH& unlock() {
      if (process) *process << "разблокировка памяти для записи" << std::endl;
      // static_cast<mcu::FLASH*>(this)->unlock(); // внутренние задержки жутко тормозили тесты
      this->like_CMSIS().CR &= ~FLASH_CR_LOCK_Msk;
      return *this;
   }

   FLASH& set_progMode() {
      if (process) *process << "установка режима программирования" << std::endl;
      static_cast<mcu::FLASH*>(this)->set_progMode();
      return *this;
   }

   FLASH& clear_flag_endOfProg() {
      if (process) *process << "сброс флага окончания записи" << std::endl;
      static_cast<mcu::FLASH*>(this)->clear_flag_endOfProg();
      return *this;
   }

   FLASH& set (ProgSize v) {
      if (process) *process << "установка размера записи " << v << std::endl;
      static_cast<mcu::FLASH*>(this)->set(v);
      return *this;
   }

   FLASH& en_interrupt_endOfProg() {
      if (process) *process << "разрешение прерывания по окончанию записи" << std::endl;
      static_cast<mcu::FLASH*>(this)->en_interrupt_endOfProg();
      return *this;
   }

   FLASH& start_erase (Sector v) {
      if (process) *process << "запуск стирания сектора " << static_cast<size_t>(v) << std::endl;
      static_cast<mcu::FLASH*>(this)->start_erase(v);
      if (erase) erase(v);
      return *this;
   }

   // моделирование задержки записи в 3 мс
   bool is_endOfProg() { return not ((eop_count += 1) %= 3); }

   template<Sector s> static size_t address() { return reinterpret_cast<size_t>(&memory<s>); }

};

} // namespace mock {

#if defined(USE_PERIPH_MOCK)
namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::FLASH, mock::FLASH&> make_reference() { return mock::FLASH::make(); }
}
#endif
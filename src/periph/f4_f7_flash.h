#pragma once

#if   defined(STM32F4)
   #include "f4_bits_flash.h"
#elif defined(STM32F7)
   #include "f7_bits_flash.h"
#endif

namespace mcu {

class FLASH {
protected:
   volatile FLASH_bits::ACR ACR;     // access control register,   offset: 0x00
   volatile uint32_t        KEYR;    // key register,              offset: 0x04
   volatile uint32_t        OPTKEYR; // option key register,       offset: 0x08
   volatile FLASH_bits::SR  SR;      // status register,           offset: 0x0C
   volatile FLASH_bits::CR  CR;      // control register,          offset: 0x10
   volatile uint32_t        OPTCR;   // option control register ,  offset: 0x14
   volatile uint32_t        OPTCR1;  // option control register 1, offset: 0x18
public:
   using CMSIS_type   = FLASH_TypeDef;
   using Latency      = FLASH_bits::ACR::Latency;
   using ProgSize     = FLASH_bits::CR::ProgSize;
   using Sector       = FLASH_bits::CR::Sector;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   FLASH& set (Latency v)            { ACR.LATENCY = v;    return *this; }
   FLASH& lock()                     { CR.LOCK     = true; return *this; }
   bool   is_lock()                  { return CR.LOCK;                   }
   FLASH& unlock();
   FLASH& set_progMode()             { CR.PG       = true; return *this; }
   bool   is_endOfProg()             { return SR.EOP;                    }
   FLASH& clear_flag_endOfProg()     { SR.EOP      = true; return *this; }
   bool   is_busy()                  { return SR.BSY;                    }
   FLASH& set (ProgSize v)           { CR.PSIZE    = v;    return *this; }
   FLASH& en_interrupt_endOfProg()   { CR.EOPIE    = true; return *this; }

   template<Sector> FLASH& start_erase();

   template<Sector> static constexpr size_t address();
   template<Sector> static constexpr size_t size();
};


#if not defined(USE_PERIPH_MOCK)
template<Periph p> std::enable_if_t<p == Periph::FLASH, FLASH&> make_reference() { return *reinterpret_cast<FLASH*>(FLASH_R_BASE); }
#endif







FLASH& FLASH::unlock()
{
   constexpr uint32_t Key1 = 0x45670123;
   constexpr uint32_t Key2 = 0xCDEF89AB;
   if (is_lock()) {
      KEYR = Key1;
      IF_TEST_WAIT_MS(10);
      KEYR = Key2;
   }
   return *this;
}


template<FLASH::Sector s>
FLASH& FLASH::start_erase()
{
   CR.SER  = true;
   IF_TEST_WAIT_MS(1);
   CR.SNB  = s;
   IF_TEST_WAIT_MS(1);
   CR.STRT = true;
   return *this;
}


template <FLASH::Sector v> constexpr size_t FLASH::address()
{
   return 
      v == Sector::_0  ? 0x08000000 :
      v == Sector::_1  ? 0x08004000 :
      v == Sector::_2  ? 0x08008000 :
      v == Sector::_3  ? 0x0800C000 :
      v == Sector::_4  ? 0x08010000 :
      v == Sector::_5  ? 0x08020000 :
      v == Sector::_6  ? 0x08040000 :
      v == Sector::_7  ? 0x08060000 :
      #if defined (STM32F4)
      v == Sector::_8  ? 0x08080000 :
      v == Sector::_9  ? 0x080A0000 :
      v == Sector::_10 ? 0x080C0000 :
      v == Sector::_11 ? 0x080E0000 :
      #endif
      0; // такого не может быть
}


template <FLASH::Sector v> constexpr size_t FLASH::size()
{
   return 
      v >= 0 and v < 4  ?  16*1024 :
      v == 4            ?  64*1024 :
      v > 4 and v <= 11 ? 128*1024 :
      0; // такого не может быть
                              
}



} // namespace mcu {
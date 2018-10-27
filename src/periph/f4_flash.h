#pragma once

#include "f4_bits_flash.h"

namespace mcu {

class FLASH {
   __IO FLASH_bits::ACR ACR;     // access control register,   offset: 0x00
   __IO uint32_t        KEYR;    // key register,              offset: 0x04
   __IO uint32_t        OPTKEYR; // option key register,       offset: 0x08
   __IO FLASH_bits::SR  SR;      // status register,           offset: 0x0C
   __IO FLASH_bits::CR  CR;      // control register,          offset: 0x10
   __IO uint32_t        OPTCR;   // option control register ,  offset: 0x14
   __IO uint32_t        OPTCR1;  // option control register 1, offset: 0x18
public:
   using CMSIS_type   = FLASH_TypeDef;

   FLASH& set (Latency v)            { ACR.LATENCY = v;    return *this; }
   FLASH& lock()                     { CR.LOCK     = true; return *this; }
   bool is_lock()                    { return CR.LOCK;     }
   FLASH& unlock();
   FLASH& setProgMode()              { CR.PG       = true; return *this; }
   bool is_endOfProg()               { return SR.EOP;      }
   FLASH& clearEndOfProgFlag()       { SR.EOP      = true; return *this; }
   bool is_busy()                    { return SR.BSY;      }
   template<ProgSize v> FLASH& set() { CR.PSIZE    = v;    return *this; }
   FLASH& endOfProgInterruptEn()     { CR.EOPIE    = true; return *this; }

   template <Sector> void startErase();

   template<Sector> static constexpr uint32_t address();
   template<Sector> static constexpr int32_t  size();
   template<Sector> static constexpr Sector   next();
};







template<Periph p> std::enable_if_t<p == Periph::FLASH, FLASH&> make_reference() { return *reinterpret_cast<FLASH*>(FLASH_R_BASE); }

} // namespace mcu {
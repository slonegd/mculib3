#pragma once

#include "f1_bits_flash.h"

namespace mcu {

class FLASH {
   __IO FLASH_bits::ACR ACR;     // access control register,   offset: 0x00
   __IO uint32_t        KEYR;    // key register,              offset: 0x04
   __IO uint32_t        OPTKEYR; // option key register,       offset: 0x08
   __IO uint32_t        SR;      // status register,           offset: 0x0C
   __IO uint32_t        CR;      // control register,          offset: 0x10
   __IO uint32_t        AR;      // Flash address register     offset: 0x14
   __IO uint32_t        RESERVED;
   __IO uint32_t        OBR;     // Option byte register       offset: 0x1C
   __IO uint32_t        WRPR;    // Write protection register  offset: 0x20
public:
   using CMSIS_type = FLASH_TypeDef;
   using Latency    = FLASH_bits::ACR::Latency;
   // using ProgSize     = FLASH_bits::CR::ProgSize;
   // using Sector       = FLASH_bits::CR::Sector;

   FLASH& set (Latency v)            { ACR.LATENCY = v;    return *this; }
   FLASH& enable ()                  { ACR.PRFTBE = true;  return *this; }
   // FLASH& lock()                     { CR.LOCK     = true; return *this; }
   // bool   is_lock()                  { return CR.LOCK;                   }
   // FLASH& unlock();
   // FLASH& set_progMode()             { CR.PG       = true; return *this; }
   // bool   is_endOfProg()             { return SR.EOP;                    }
   // FLASH& clear_flag_endOfProg()     { SR.EOP      = true; return *this; }
   // bool   is_busy()                  { return SR.BSY;                    }
   // FLASH& set (ProgSize v)           { CR.PSIZE    = v;    return *this; }
   // FLASH& en_interrupt_endOfProg()   { CR.EOPIE    = true; return *this; }

   // FLASH& start_erase (Sector);

   // template<Sector> static constexpr size_t address();
   // template<Sector> static constexpr size_t size();
};


template<Periph p> std::enable_if_t<p == Periph::FLASH, FLASH&> make_reference() { return *reinterpret_cast<FLASH*>(FLASH_R_BASE); }



} // namespace mcu {
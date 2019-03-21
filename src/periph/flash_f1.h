#pragma once

#include "bits_flash_f1.h"

namespace mcu {

class FLASH {
protected:
   volatile FLASH_bits::ACR ACR;      // FLASH access control register, offset: 0x00
   volatile uint32_t        KEYR;     // FLASH key register,            offset: 0x04
   volatile uint32_t        OPTKEYR;  // FLASH OPT key register,        offset: 0x08
   volatile FLASH_bits::SR  SR;       // FLASH status register,         offset: 0x0C
   volatile FLASH_bits::CR  CR;       // FLASH control register,        offset: 0x10
   volatile uint32_t        AR;       // FLASH address register,        offset: 0x14
   volatile uint32_t        RESERVED; //  Reserved,                             0x18
   volatile uint32_t        OBR;      // FLASH option bytes register,   offset: 0x1C
   volatile uint32_t        WRPR;     // FLASH option bytes register,   offset: 0x20
public:
   using CMSIS_type = FLASH_TypeDef;
   using Latency    = FLASH_bits::ACR::Latency;
   /// фактически это страница, чтобы было единообразно с F4
   enum Sector {  _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,
                 _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,
                 _20, _21, _22, _23, _24, _25, _26, _27, _28, _29,
                 _30, _31, _32, _33, _34, _35, _36, _37, _38, _39,
                 _40, _41, _42, _43, _44, _45, _46, _47, _48, _49,
                 _50, _51, _52, _53, _54, _55, _56, _57, _58, _59,
                 _60, _61, _62, _63, _64, _65, _66, _67, _68, _69,
                 _70, _71, _72, _73, _74, _75, _76, _77, _78, _79,
                 _80, _81, _82, _83, _84, _85, _86, _87, _88, _89,
                 _90, _91, _92, _93, _94, _95, _96, _97, _98, _99,
                _100,_101,_102,_103,_104,_105,_106,_107,_108,_109,
                _110,_111,_112,_113,_114,_115,_116,_117,_118,_119,
                _120,_121,_122,_123,_124,_125,_126,_127
   };

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   FLASH& set (Latency v)            { ACR.LATENCY = v;    return *this; }
   FLASH& enable ()                  { ACR.PRFTBE = true;  return *this; }
   FLASH& lock()                     { CR.LOCK     = true; return *this; }
   bool   is_lock()                  { return CR.LOCK;                   }
   FLASH& unlock();
   FLASH& set_progMode()             { CR.PG       = true; return *this; }
   bool   is_endOfProg()             { return SR.EOP;                    }
   FLASH& clear_flag_endOfProg()     { SR.EOP      = true; return *this; }
   bool   is_busy()                  { return SR.BSY;                    }
   FLASH& en_interrupt_endOfProg()   { CR.EOPIE    = true; return *this; }

   template<Sector> FLASH& start_erase();

   template<Sector s> static constexpr size_t address() { return 0x08000000 + 1024 * s; }
   template<Sector>   static constexpr size_t size()    { return 1024; }
};


#if not defined(USE_MOCK_FLASH)
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
   CR.PER  = true;
   IF_TEST_WAIT_MS(1);
   AR = address<s>();
   IF_TEST_WAIT_MS(1);
   CR.STRT = true;
   return *this;
}



} // namespace mcu {
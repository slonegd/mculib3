#pragma once

#include "periph.h"
#include "bits_afio_f1.h"

namespace mcu {

class AFIO {
   volatile AFIO_bits::EVCR   EVCR;      // Event control register                         offset: 0x00
   volatile AFIO_bits::MAPR   MAPR;      // AF remap and debug I/O configuration register  offset: 0x04
   volatile AFIO_bits::EXTICR EXTICR;    // External interrupt configuration register      offset: 0x08
            uint32_t          RESERVED0; //
   volatile AFIO_bits::MAPR2  MAPR2;     // AF remap and debug I/O configuration register2 offset: 0x1C

public:
   using CMSIS_type = AFIO_TypeDef;
   using Port       = AFIO_bits::Port;
   using Pin        = AFIO_bits::EVCR::Pin;
   using Remap      = AFIO_bits::MAPR::Remap;
   using SWJ        = AFIO_bits::MAPR::SWJ;

   // AFIO& event_enable()        {EVCR.EVOE = true;   return *this;}
   AFIO& set_JTAG (SWJ v) {MAPR.SWJ_CFG = v; return *this;}

   template<Periph> AFIO& remap();
   template<Periph, Remap> AFIO& remap();

   
};

#if not defined(USE_MOCK_AFIO)
SFINAE(AFIO, AFIO)  make_reference() { return *reinterpret_cast<AFIO*>(AFIO_BASE); }
#endif














template<Periph p> AFIO& AFIO::remap()
{
   if      constexpr (p == Periph::USART1) {MAPR.USART1_REMAP = true; return *this;}
   else if constexpr (p == Periph::USART2) {MAPR.USART2_REMAP = true; return *this;}
}

template<Periph p, AFIO::Remap r> AFIO& AFIO::remap()
{
   if      constexpr (p == Periph::USART3) {MAPR.USART3_REMAP = r; return *this;}
}






} //namespace mcu
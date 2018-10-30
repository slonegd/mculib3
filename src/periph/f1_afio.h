#pragma once

#include "f1_bits_afio.h"

namespace mcu {

class AFIO {
   __IO AFIO_bits::EVCR   EVCR;      // Event control register                         offset: 0x00
   __IO AFIO_bits::MAPR   MAPR;      // AF remap and debug I/O configuration register  offset: 0x04
   __IO AFIO_bits::EXTICR EXTICR;    // External interrupt configuration register      offset: 0x08
        uint32_t          RESERVED0; //
   __IO AFIO_bits::MAPR2  MAPR2;     // AF remap and debug I/O configuration register2 offset: 0x1C

public:
   using CMSIS_type = AFIO_TypeDef;
   using Port       = AFIO_bits::Port;
   using Pin        = AFIO_bits::EVCR::Pin;
   using Remap      = AFIO_bits::MAPR::Remap;

   template<Periph p, Periph v = Periph::RCC> AFIO& clock_enable() { make_reference<v>().template clock_enable<p>(); return *this; }

   AFIO& event_enable() {EVCR.EVOE = true;}

   template<Periph> AFIO& remap();
   template<Periph, Remap> AFIO& remap();
};

template<Periph p> std::enable_if_t<p == Periph::AFIO, AFIO&> make_reference() { return *reinterpret_cast<AFIO*>(AFIO_BASE); }















template<Periph p> AFIO& AFIO::remap()
{
   if      constexpr (p == Periph::USART1) MAPR.USART1_REMAP = true;
   else if constexpr (p == Periph::USART2) MAPR.USART2_REMAP = true;
}

template<Periph p, AFIO::Remap r> AFIO& AFIO::remap()
{
   if      constexpr (p == Periph::USART3) MAPR.USART3_REMAP = r;
}






} //namespace mcu
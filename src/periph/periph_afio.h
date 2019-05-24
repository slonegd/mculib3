#pragma once

#include "periph.h"
#include "bits_afio_f1.h"

namespace mcu {

class AFIO {
    volatile AFIO_bits::EVCR   EVCR;      // Event control register                         offset: 0x00
    volatile AFIO_bits::MAPR   MAPR;      // AF remap and debug I/O configuration register  offset: 0x04
    volatile AFIO_bits::EXTICR EXTICR;    // External interrupt configuration register      offset: 0x08
    volatile uint32_t          RESERVED0; //
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

    template<class Pin_> AFIO& set_external_interrupt();

    
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

template<class Pin_> AFIO& AFIO::set_external_interrupt()
{
    constexpr auto port =
        Pin_::periph == Periph::GPIOA ? Port::PA :
        Pin_::periph == Periph::GPIOB ? Port::PB :
        Pin_::periph == Periph::GPIOC ? Port::PC :
        Pin_::periph == Periph::GPIOD ? Port::PD :
        Pin_::periph == Periph::GPIOE ? Port::PE :
        // Pin_::periph == Periph::GPIOF ? Port::PF : // placeholder
        // Pin_::periph == Periph::GPIOG ? Port::PG : // placeholder
        Port::PA; // this is never happens

    if constexpr (Pin_::n ==  0) EXTICR.EXTI0  = port;
    if constexpr (Pin_::n ==  1) EXTICR.EXTI1  = port;
    if constexpr (Pin_::n ==  2) EXTICR.EXTI2  = port;
    if constexpr (Pin_::n ==  3) EXTICR.EXTI3  = port;
    if constexpr (Pin_::n ==  4) EXTICR.EXTI4  = port;
    if constexpr (Pin_::n ==  5) EXTICR.EXTI5  = port;
    if constexpr (Pin_::n ==  6) EXTICR.EXTI6  = port;
    if constexpr (Pin_::n ==  7) EXTICR.EXTI7  = port;
    if constexpr (Pin_::n ==  8) EXTICR.EXTI8  = port;
    if constexpr (Pin_::n ==  9) EXTICR.EXTI9  = port;
    if constexpr (Pin_::n == 10) EXTICR.EXTI10 = port;
    if constexpr (Pin_::n == 11) EXTICR.EXTI11 = port;
    if constexpr (Pin_::n == 12) EXTICR.EXTI12 = port;
    if constexpr (Pin_::n == 13) EXTICR.EXTI13 = port;
    if constexpr (Pin_::n == 14) EXTICR.EXTI14 = port;
    if constexpr (Pin_::n == 15) EXTICR.EXTI15 = port;

    return *this;
}






} //namespace mcu
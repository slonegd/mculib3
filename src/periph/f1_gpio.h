#pragma once

#include "periph.h"
#include "f1_bits_gpio.h"
#include "rcc.h"

namespace mcu {

class GPIO {
  __IO GPIO_bits::CR CR;   // Port configuration register      offset: 0x00
  __IO GPIO_bits::DR IDR;  // Port input data register         offset: 0x08
  __IO GPIO_bits::DR ODR;  // Port output data register        offset: 0x0C
  __IO uint32_t      BSRR; // Port bit set/reset register      offset: 0x10
  __IO uint32_t      BRR;  // Port bit reset register          offset: 0x14
  __IO uint32_t      LCKR; // Port configuration lock register offset: 0x18

public:
   using CMSIS_type = GPIO_TypeDef;
   using Pin_mode   = GPIO_bits::CR::Pin_mode;

   template<Periph p, Periph v = Periph::RCC> GPIO& clock_enable() { make_reference<v>().template clock_enable<p>(); return *this; }

   void set     (size_t n) { BSRR |= (1 << n);              }
   void clear   (size_t n) { BSRR |= (1 << (n + 16));       }
   bool is_set  (size_t n) { return IDR.reg & (1 << n);     }
   void toggle  (size_t n) { is_set(n) ? clear(n) : set(n); }

   template<size_t> GPIO& set (Pin_mode);
};





















template<Periph p> std::enable_if_t<p == Periph::GPIOA, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOA_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOB, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOB_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOC, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOC_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOD, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOD_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOE, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOE_BASE); }

template<size_t n> GPIO& GPIO::set (Pin_mode v) 
{
   if      constexpr (n == 0)  { CR.CNF_MODE0  = v; return *this; }
   else if constexpr (n == 1)  { CR.CNF_MODE1  = v; return *this; }
   else if constexpr (n == 2)  { CR.CNF_MODE2  = v; return *this; }
   else if constexpr (n == 3)  { CR.CNF_MODE3  = v; return *this; }
   else if constexpr (n == 4)  { CR.CNF_MODE4  = v; return *this; }
   else if constexpr (n == 5)  { CR.CNF_MODE5  = v; return *this; }
   else if constexpr (n == 6)  { CR.CNF_MODE6  = v; return *this; }
   else if constexpr (n == 7)  { CR.CNF_MODE7  = v; return *this; }
   else if constexpr (n == 8)  { CR.CNF_MODE8  = v; return *this; }
   else if constexpr (n == 9)  { CR.CNF_MODE9  = v; return *this; }
   else if constexpr (n == 10) { CR.CNF_MODE10 = v; return *this; }
   else if constexpr (n == 11) { CR.CNF_MODE11 = v; return *this; }
   else if constexpr (n == 12) { CR.CNF_MODE12 = v; return *this; }
   else if constexpr (n == 13) { CR.CNF_MODE13 = v; return *this; }
   else if constexpr (n == 14) { CR.CNF_MODE14 = v; return *this; }
   else if constexpr (n == 15) { CR.CNF_MODE15 = v; return *this; }
}

} //namespace mcu
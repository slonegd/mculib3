#pragma once

#include "periph.h"
#include "f0_f4_bits_gpio.h"
#include "rcc.h"

enum class PinMode { Input, Output, Alternate_1 };

namespace mcu {

class GPIO {
   __IO GPIO_bits::MODER   MODER;   // mode register,                offset: 0x00
   __IO GPIO_bits::OTYPER  OTYPER;  // output type register,         offset: 0x04
   __IO GPIO_bits::OSPEEDR OSPEEDR; // output speed register,        offset: 0x08
   __IO GPIO_bits::PUPDR   PUPDR;   // pull-up/pull-down register,   offset: 0x0C
   __IO GPIO_bits::DR      IDR;     // input data register,          offset: 0x10
   __IO GPIO_bits::DR      ODR;     // output data register,         offset: 0x14
   __IO uint32_t           BSRR;    // bit set/reset register,       offset: 0x18
   __IO uint32_t           LCKR;    // configuration lock register,  offset: 0x1C
   __IO GPIO_bits::AFR     AFR;     // alternate function registers, offset: 0x20-0x24
public:
   using Mode = GPIO_bits::MODER::Mode;
   using AF   = GPIO_bits::  AFR::AF;

   template<Periph, class RCC = RCC> static GPIO& make_reference();

   void set    (size_t n) { BSRR |= (1 << n);              }
   void clear  (size_t n) { BSRR |= (1 << (n + 16));       }
   bool is_set (size_t n) { return IDR.reg & (1 << n);     }
   void toggle (size_t n) { is_set(n) ? clear(n) : set(n); }

   template<size_t> GPIO& set (Mode mode);
   template<size_t> GPIO& set (AF af);
   template<size_t, PinMode> void init();
};

} // namespace mcu {
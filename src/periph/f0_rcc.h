#pragma once

#include "f0_bits_rcc.h"

namespace mcu {

class RCC {
   __IO RCC_bits::CR      CR;         // clock control register,                offset: 0x00
   __IO RCC_bits::CFGR    CFGR;       // clock configuration register,          offset: 0x04
   __IO uint32_t          CIR;        // clock interrupt register,              offset: 0x08
   __IO uint32_t          APB2RSTR;   // APB2 peripheral reset register,        offset: 0x0C
   __IO uint32_t          APB1RSTR;   // APB1 peripheral reset register,        offset: 0x10
   __IO RCC_bits::AHBENR  AHBENR;     // AHB peripheral clock register,         offset: 0x14
   __IO RCC_bits::APB2ENR APB2ENR;    // APB2 peripheral clock enable register, offset: 0x18
   __IO RCC_bits::APB1ENR APB1ENR;    // APB1 peripheral clock enable register, offset: 0x1C
   __IO uint32_t          BDCR;       // Backup domain control register,        offset: 0x20
   __IO uint32_t          CSR;        // clock control & status register,       offset: 0x24
   __IO uint32_t          AHBRSTR;    // AHB peripheral reset register,         offset: 0x28
   __IO uint32_t          CFGR2;      // clock configuration register 2,        offset: 0x2C
   __IO uint32_t          CFGR3;      // clock configuration register 3,        offset: 0x30
   __IO uint32_t          CR2;        // clock control register 2,              offset: 0x34


public:
   using CMSIS_type    = RCC_TypeDef;
   using AHBprescaler  = RCC_bits::CFGR::AHBprescaler;
   using APBprescaler  = RCC_bits::CFGR::APBprescaler;
   using SystemClock   = RCC_bits::CFGR::SystemClock;
   using PLLsource     = RCC_bits::CFGR::PLLsource;
   using PLLmultiplier = RCC_bits::CFGR::PLLmultiplier;

   RCC& set (AHBprescaler  v) { CFGR.HPRE   = v; return *this; }
   RCC& set (APBprescaler  v) { CFGR.PPRE   = v; return *this; }
   RCC& set (SystemClock   v) { CFGR.SW     = v; return *this; }
   RCC& set (PLLsource     v) { CFGR.PLLSRC = v; return *this; }
   RCC& set (PLLmultiplier v) { CFGR.PLLMUL = v; return *this; }

   RCC& on_PLL        () { CR.PLLON = true;         return *this; }
   RCC& wait_PLL_ready() { while (not CR.PLLRDY) {} return *this; }

   template<Periph p> void clock_enable()
   {
      if      constexpr (p == Periph::GPIOA) AHBENR .IOPAEN  = true;
      else if constexpr (p == Periph::GPIOB) AHBENR .IOPBEN  = true;
      else if constexpr (p == Periph::GPIOC) AHBENR .IOPCEN  = true;
      else if constexpr (p == Periph::GPIOD) AHBENR .IOPDEN  = true;
      else if constexpr (p == Periph::GPIOF) AHBENR .IOPFEN  = true;
      else if constexpr (p == Periph::TIM1)  APB2ENR.TIM1EN  = true;
      else if constexpr (p == Periph::TIM3)  APB1ENR.TIM3EN  = true;
      else if constexpr (p == Periph::TIM14) APB1ENR.TIM14EN = true;
      else if constexpr (p == Periph::TIM16) APB2ENR.TIM16EN = true;
      else if constexpr (p == Periph::TIM17) APB2ENR.TIM17EN = true;
   }
};


template<Periph p> std::enable_if_t<p == Periph::RCC, RCC&> make_reference() { return *reinterpret_cast<RCC*>(RCC_BASE); }

} // namespace mcu {
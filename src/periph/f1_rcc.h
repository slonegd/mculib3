#pragma once

#include "f1_bits_rcc.h"

namespace mcu {

class RCC {
   __IO RCC_bits::CR      CR;       // RCC clock control register,                offset: 0x00
   __IO RCC_bits::CFGR    CFGR;     // RCC clock configuration register,          offset: 0x04
   __IO uint32_t          CIR;      // RCC clock interrupt register,              offset: 0x08
   __IO uint32_t          APB2RSTR; // RCC APB2 peripheral reset register,        offset: 0x0C
   __IO uint32_t          APB1RSTR; // RCC APB1 peripheral reset register,        offset: 0x10
   __IO RCC_bits::AHBENR  AHBENR;   // RCC AHB peripheral clock register,         offset: 0x14
   __IO RCC_bits::APB2ENR APB2ENR;  // RCC APB2 peripheral clock enable register, offset: 0x18
   __IO RCC_bits::APB1ENR APB1ENR;  // RCC APB1 peripheral clock enable register, offset: 0x1C
   __IO uint32_t          BDCR;     // RCC Backup domain control register,        offset: 0x20
   __IO uint32_t          CSR;      // RCC clock control & status register,       offset: 0x24

public:
   using CMSIS_type    = RCC_TypeDef;
   using AHBprescaler  = RCC_bits::CFGR::AHBprescaler;
   using APBprescaler  = RCC_bits::CFGR::APBprescaler;
   using ADCprescaler  = RCC_bits::CFGR::ADCprescaler;
   using SystemClock   = RCC_bits::CFGR::SystemClock;
   using PLLsource     = RCC_bits::CFGR::PLLsource;
   using PLLmultiplier = RCC_bits::CFGR::PLLmultiplier;

   RCC& set       (AHBprescaler  v) { CFGR.HPRE   = v; return *this; }
   RCC& set_APB1  (APBprescaler  v) { CFGR.PPRE1  = v; return *this; }
   RCC& set_APB2  (APBprescaler  v) { CFGR.PPRE2  = v; return *this; }
   RCC& set       (ADCprescaler  v) { CFGR.ADCPRE = v; return *this; }
   RCC& set       (SystemClock   v) { CFGR.SW     = v; return *this; }
   RCC& set       (PLLsource     v) { CFGR.PLLSRC = v; return *this; }
   RCC& set       (PLLmultiplier v) { CFGR.PLLMUL = v; return *this; }

   RCC& on_HSE        () { CR.HSEON = true;         return *this; }
   RCC& wait_HSE_ready() { while (not CR.HSERDY) {} return *this; }
   RCC& on_PLL        () { CR.PLLON = true;         return *this; }
   RCC& wait_PLL_ready() { while (not CR.PLLRDY) {} return *this; }

   size_t get_APB_clock (APBprescaler v)
   {return v == APBprescaler::APBnotdiv ? F_CPU     :
           v == APBprescaler::APBdiv2   ? F_CPU / 2 :
           v == APBprescaler::APBdiv4   ? F_CPU / 4 :
           v == APBprescaler::APBdiv8   ? F_CPU / 8 :
                                          F_CPU / 16;}

   size_t get_APB1_clock(){return get_APB_clock (CFGR.PPRE1);}
   size_t get_APB2_clock(){return get_APB_clock (CFGR.PPRE2);}



   template<Periph p> void clock_enable()
   {
      if      constexpr (p == Periph::GPIOA) APB2ENR.IOPAEN = true;
      else if constexpr (p == Periph::GPIOB) APB2ENR.IOPBEN = true;
      else if constexpr (p == Periph::GPIOC) APB2ENR.IOPCEN = true;
      else if constexpr (p == Periph::GPIOD) APB2ENR.IOPDEN = true;
      else if constexpr (p == Periph::GPIOE) APB2ENR.IOPEEN = true;
      else if constexpr (p == Periph::AFIO ) APB2ENR.AFIOEN = true;

      else if constexpr (p == Periph::USART1) APB2ENR.USART1EN = true;
      else if constexpr (p == Periph::USART2) APB1ENR.USART2EN = true;
      else if constexpr (p == Periph::USART3) APB1ENR.USART3EN = true;
   }
};

template<Periph p> std::enable_if_t<p == Periph::RCC, RCC&> make_reference() { return *reinterpret_cast<RCC*>(RCC_BASE); }


} //namespace mcu
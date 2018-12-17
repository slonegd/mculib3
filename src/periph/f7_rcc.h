#pragma once

#include "f7_bits_rcc.h"

namespace mcu {

class RCC {
   volatile RCC_bits::CR      CR;           // clock control register,                                  offset: 0x00
   volatile RCC_bits::PLLCFGR PLLCFGR;      // PLL configuration register,                              offset: 0x04
   volatile RCC_bits::CFGR    CFGR;         // clock configuration register,                            offset: 0x08
   volatile uint32_t          CIR;          // clock interrupt register,                                offset: 0x0C
   volatile uint32_t          AHB1RSTR;     // AHB1 peripheral reset register,                          offset: 0x10
   volatile uint32_t          AHB2RSTR;     // AHB2 peripheral reset register,                          offset: 0x14
   volatile uint32_t          AHB3RSTR;     // AHB3 peripheral reset register,                          offset: 0x18
   volatile uint32_t          RESERVED0;    // Reserved, 0x1C                                                                   
   volatile uint32_t          APB1RSTR;     // APB1 peripheral reset register,                          offset: 0x20
   volatile uint32_t          APB2RSTR;     // APB2 peripheral reset register,                          offset: 0x24
   volatile uint32_t          RESERVED1[2]; // Reserved, 0x28-0x2C                                                              
   volatile RCC_bits::AHB1ENR AHB1ENR;      // AHB1 peripheral clock register,                          offset: 0x30
   volatile uint32_t          AHB2ENR;      // AHB2 peripheral clock register,                          offset: 0x34
   volatile uint32_t          AHB3ENR;      // AHB3 peripheral clock register,                          offset: 0x38
   volatile uint32_t          RESERVED2;    // Reserved, 0x3C                                                                   
   volatile RCC_bits::APBENR  APBENR;       // APB peripheral clock enable register,                    offset: 0x40
   volatile uint32_t          RESERVED3[2]; // Reserved, 0x48-0x4C                                                              
   volatile uint32_t          AHB1LPENR;    // AHB1 peripheral clock enable in low power mode register, offset: 0x50
   volatile uint32_t          AHB2LPENR;    // AHB2 peripheral clock enable in low power mode register, offset: 0x54
   volatile uint32_t          AHB3LPENR;    // AHB3 peripheral clock enable in low power mode register, offset: 0x58
   volatile uint32_t          RESERVED4;    // Reserved, 0x5C                                                                   
   volatile uint32_t          APB1LPENR;    // APB1 peripheral clock enable in low power mode register, offset: 0x60
   volatile uint32_t          APB2LPENR;    // APB2 peripheral clock enable in low power mode register, offset: 0x64
   volatile uint32_t          RESERVED5[2]; // Reserved, 0x68-0x6C                                                              
   volatile uint32_t          BDCR;         // Backup domain control register,                          offset: 0x70
   volatile uint32_t          CSR;          // clock control & status register,                         offset: 0x74
   volatile uint32_t          RESERVED6[2]; // Reserved, 0x78-0x7C                                                              
   volatile uint32_t          SSCGR;        // spread spectrum clock generation register,               offset: 0x80
   volatile uint32_t          PLLI2SCFGR;   // PLLI2S configuration register,                           offset: 0x84
   volatile uint32_t          PLLSAICFGR;   // PLLSAI configuration register,                           offset: 0x88
   volatile uint32_t          DCKCFGR1;     // Dedicated Clocks configuration register1,                offset: 0x8C
   volatile uint32_t          DCKCFGR2;     // Dedicated Clocks configuration register 2,               offset: 0x90

public:
   using CMSIS_type   = RCC_TypeDef;
   using AHBprescaler = RCC_bits::   CFGR::AHBprescaler;
   using APBprescaler = RCC_bits::   CFGR::APBprescaler;
   using SystemClock  = RCC_bits::   CFGR::SystemClock;
   using PLLPdiv      = RCC_bits::PLLCFGR::PLLPdiv;
   using PLLsource    = RCC_bits::PLLCFGR::PLLsource;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   RCC& set       (AHBprescaler v) { CFGR.HPRE      = v; return *this; }
   RCC& set_APB1  (APBprescaler v) { CFGR.PPRE1     = v; return *this; }
   RCC& set_APB2  (APBprescaler v) { CFGR.PPRE2     = v; return *this; }
   RCC& set       (SystemClock  v) { CFGR.SW        = v; return *this; }
   RCC& set       (PLLPdiv      v) { PLLCFGR.PLLP   = v; return *this; }
   RCC& set       (PLLsource    v) { PLLCFGR.PLLSRC = v; return *this; }

   size_t get_APB1_clock() { return get_APB_clock (CFGR.PPRE1); }
   size_t get_APB2_clock() { return get_APB_clock (CFGR.PPRE2); }
   
   template<int v> RCC& set_PLLM() { static_assert (v >= 2  and v <= 63 ); PLLCFGR.PLLM = v; return *this; }
   template<int v> RCC& set_PLLN() { static_assert (v >= 50 and v <= 432); PLLCFGR.PLLN = v; return *this; }
   template<int v> RCC& set_PLLQ() { static_assert (v >= 2  and v <= 15 ); PLLCFGR.PLLQ = v; return *this; }

   RCC& on_HSE        () { CR.HSEON = true;         return *this; }
   RCC& wait_HSE_ready() { while (not CR.HSERDY) {} return *this; }
   RCC& on_PLL        () { CR.PLLON = true;         return *this; }
   RCC& wait_PLL_ready() { while (not CR.PLLRDY) {} return *this; }

   template<Periph p> RCC& clock_enable()
   {
      if      constexpr (p == Periph::GPIOA) AHB1ENR.GPIOAEN = true;
      else if constexpr (p == Periph::GPIOB) AHB1ENR.GPIOBEN = true;
      else if constexpr (p == Periph::GPIOC) AHB1ENR.GPIOCEN = true;
      else if constexpr (p == Periph::GPIOD) AHB1ENR.GPIODEN = true;
      else if constexpr (p == Periph::GPIOE) AHB1ENR.GPIOEEN = true;
      else if constexpr (p == Periph::GPIOF) AHB1ENR.GPIOFEN = true;
      else if constexpr (p == Periph::GPIOG) AHB1ENR.GPIOGEN = true;
      else if constexpr (p == Periph::GPIOH) AHB1ENR.GPIOHEN = true;
      else if constexpr (p == Periph::GPIOI) AHB1ENR.GPIOIEN = true;

      else if constexpr (p == Periph::USART1) APBENR.USART1EN = true;
      else if constexpr (p == Periph::USART2) APBENR.USART2EN = true;
      else if constexpr (p == Periph::USART3) APBENR.USART3EN = true;
      else if constexpr (p == Periph::USART4) APBENR.UART4EN  = true;
      else if constexpr (p == Periph::USART5) APBENR.UART5EN  = true;
      else if constexpr (p == Periph::USART6) APBENR.USART6EN = true;
      else if constexpr (p == Periph::USART7) APBENR.UART7EN  = true;
      else if constexpr (p == Periph::USART8) APBENR.UART8EN  = true;

      return *this;
   }



private:
   size_t get_APB_clock (APBprescaler v)
   {
      return v == APBprescaler::APBnotdiv ? F_CPU     :
             v == APBprescaler::APBdiv2   ? F_CPU / 2 :
             v == APBprescaler::APBdiv4   ? F_CPU / 4 :
             v == APBprescaler::APBdiv8   ? F_CPU / 8 :
                                            F_CPU / 16;
   }
};

#if not defined(USE_MOCKS)
template<Periph p> std::enable_if_t<p == Periph::RCC, RCC&> make_reference() { return *reinterpret_cast<RCC*>(RCC_BASE); }
#endif

} // namespace mcu {
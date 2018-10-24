#pragma once

#include "periph.h"
#include "f0_f4_bits_gpio.h"
#include "rcc.h"



namespace mcu {

enum class PinMode { Input, Output, Alternate_1, Alternate_2, Alternate_3,
   Alternate_4, Alternate_5, Alternate_7, Alternate_8
};

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

   template<Periph p> GPIO& clock_enable() { make_reference<Periph::RCC>().clock_enable<p>(); return *this; }

   void set      (size_t n) { BSRR |= (1 << n);              }
   void clear    (size_t n) { BSRR |= (1 << (n + 16));       }
   bool is_set   (size_t n) { return IDR.reg & (1 << n);     }
   void toggle   (size_t n) { is_set(n) ? clear(n) : set(n); }

   template<size_t> GPIO& set (Mode);
   template<size_t> GPIO& set (AF);
   template<size_t, PinMode> void init();
};



















template<Periph p> std::enable_if_t<p == Periph::GPIOA, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOA_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOB, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOB_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOC, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOC_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOD, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOD_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOF, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOF_BASE); }
#if defined(STM32F4)
template<Periph p> std::enable_if_t<p == Periph::GPIOE, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOE_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOG, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOG_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOH, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOH_BASE); }
template<Periph p> std::enable_if_t<p == Periph::GPIOI, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOI_BASE); }
#endif


template<size_t n> GPIO& GPIO::set (Mode v)
{
   if      constexpr (n == 0)  { MODER.MODE0  = v; return *this; }
   else if constexpr (n == 1)  { MODER.MODE1  = v; return *this; }
   else if constexpr (n == 2)  { MODER.MODE2  = v; return *this; }
   else if constexpr (n == 3)  { MODER.MODE3  = v; return *this; }
   else if constexpr (n == 4)  { MODER.MODE4  = v; return *this; }
   else if constexpr (n == 5)  { MODER.MODE5  = v; return *this; }
   else if constexpr (n == 6)  { MODER.MODE6  = v; return *this; }
   else if constexpr (n == 7)  { MODER.MODE7  = v; return *this; }
   else if constexpr (n == 8)  { MODER.MODE8  = v; return *this; }
   else if constexpr (n == 9)  { MODER.MODE9  = v; return *this; }
   else if constexpr (n == 10) { MODER.MODE10 = v; return *this; }
   else if constexpr (n == 11) { MODER.MODE10 = v; return *this; }
   else if constexpr (n == 12) { MODER.MODE12 = v; return *this; }
   else if constexpr (n == 13) { MODER.MODE13 = v; return *this; }
   else if constexpr (n == 14) { MODER.MODE14 = v; return *this; }
   else if constexpr (n == 15) { MODER.MODE15 = v; return *this; }
}

template<size_t n> GPIO& GPIO::set (AF v)
{
   if      constexpr (n == 0)  { AFR.AF0  = v; return *this; }
   else if constexpr (n == 1)  { AFR.AF1  = v; return *this; }
   else if constexpr (n == 2)  { AFR.AF2  = v; return *this; }
   else if constexpr (n == 3)  { AFR.AF3  = v; return *this; }
   else if constexpr (n == 4)  { AFR.AF4  = v; return *this; }
   else if constexpr (n == 5)  { AFR.AF5  = v; return *this; }
   else if constexpr (n == 6)  { AFR.AF6  = v; return *this; }
   else if constexpr (n == 7)  { AFR.AF7  = v; return *this; }
   else if constexpr (n == 8)  { AFR.AF8  = v; return *this; }
   else if constexpr (n == 9)  { AFR.AF9  = v; return *this; }
   else if constexpr (n == 10) { AFR.AF10 = v; return *this; }
   else if constexpr (n == 11) { AFR.AF10 = v; return *this; }
   else if constexpr (n == 12) { AFR.AF12 = v; return *this; }
   else if constexpr (n == 13) { AFR.AF13 = v; return *this; }
   else if constexpr (n == 14) { AFR.AF14 = v; return *this; }
   else if constexpr (n == 15) { AFR.AF15 = v; return *this; }
}

template<size_t n, PinMode v> void GPIO::init()
{
   if constexpr (v == PinMode::Input) {
      set<n> (Mode::Input);

   } else if constexpr (v == PinMode::Output) {
      set<n> (Mode::Output);

   } else if constexpr (v == PinMode::Alternate_0) {
      set<n> (Mode::Alternate);
      set<n>   (AF::_0);

   } else if constexpr (v == PinMode::Alternate_1) {
      set<n> (Mode::Alternate);
      set<n>   (AF::_1);

   } else if constexpr (v == PinMode::Alternate_2) {
      set<n> (Mode::Alternate);
      set<n>   (AF::_2);

   } else if constexpr (v == PinMode::Alternate_4) {
      set<n> (Mode::Alternate);
      set<n>   (AF::_4);

   } else if constexpr (v == PinMode::Alternate_5) {
      set<n> (Mode::Alternate);
      set<n>   (AF::_5);

   } else if constexpr (v == PinMode::Alternate_7) {
      set<n> (Mode::Alternate);
      set<n>   (AF::_7);
   } else if constexpr (v == PinMode::Alternate_8) {
      set<n> (Mode::Alternate);
      set<n>   (AF::_8);
   }
}

} // namespace mcu {


#pragma once

#include "f1_bits_gpio.h"
#include "periph_rcc.h"
#include "periph_afio.h"
#include "pins.h"

#define COND(...) __VA_ARGS__

namespace mcu {

enum class PinMode {Input, Output, USART1_TX, USART1_RX, USART2_TX, USART2_RX, USART3_TX, USART3_RX};

class GPIO {
  volatile GPIO_bits::CR CR;   // Port configuration register      offset: 0x00
  volatile GPIO_bits::DR IDR;  // Port input data register         offset: 0x08
  volatile uint32_t      ODR;  // Port output data register        offset: 0x0C
  volatile uint32_t      BSRR; // Port bit set/reset register      offset: 0x10
  volatile uint32_t      BRR;  // Port bit reset register          offset: 0x14
  volatile uint32_t      LCKR; // Port configuration lock register offset: 0x18

public:
   using CMSIS_type = GPIO_TypeDef;
   using Mode       = GPIO_bits::CR::Mode;
   using Remap      = AFIO::Remap;

   auto& like_CMSIS(){return *reinterpret_cast<CMSIS_type*>(this);}

   void set        (size_t n) { BSRR |= (1 << n);              }
   void clear      (size_t n) { BSRR |= (1 << (n + 16));       }
   bool is_set     (size_t n) { return IDR.reg & (1 << n);     }
   void toggle     (size_t n) { is_set(n) ? clear(n) : set(n); }
   void atomic_write (uint32_t value) {BSRR |= value;}

   template<class Pin_, PinMode> void init();

private:
   template<size_t> GPIO& set (Mode);
};



#if not defined(USE_MOCK_GPIO)
template<Periph gpio> std::enable_if_t<gpio == Periph::GPIOA, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOA_BASE); }
template<Periph gpio> std::enable_if_t<gpio == Periph::GPIOB, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOB_BASE); }
template<Periph gpio> std::enable_if_t<gpio == Periph::GPIOC, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOC_BASE); }
template<Periph gpio> std::enable_if_t<gpio == Periph::GPIOD, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOD_BASE); }
template<Periph gpio> std::enable_if_t<gpio == Periph::GPIOE, GPIO&> make_reference() { return *reinterpret_cast<GPIO*>(GPIOE_BASE); }
#endif // #if not defined(USE_MOCK_GPIO)


















template<size_t n> GPIO& GPIO::set (Mode v) 
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

template<class Pin_, PinMode mode> void GPIO::init()
{
   if (std::is_same_v<Pin_, PA14> or
       std::is_same_v<Pin_, PA15> or
       std::is_same_v<Pin_,  PB3> or 
       std::is_same_v<Pin_,  PB4>) {
       make_reference<Periph::RCC>().template clock_enable<Periph::AFIO>();
       make_reference<Periph::AFIO>().set_JTAG(AFIO::SWJ::JTAG_off_SW_on);
   }
   
   if constexpr (mode == PinMode::Input) {
      set<Pin_::n>(Mode::In_floating);
   }

   else if constexpr (mode == PinMode::Output) {
      set<Pin_::n>(Mode::Out2MHz_push_pull);
   }
   
   else if constexpr (mode == PinMode::USART1_TX) {
      static_assert (
         std::is_same_v<Pin_, PA9> or std::is_same_v<Pin_, PB6>,
         "USART1_TX возможно только с PA9 или PB6"
      );
      make_reference<Periph::RCC>()
         .template clock_enable<Periph::AFIO>();
      set<Pin_::n>(Mode::Out2MHz_push_pull_alt);
      if (std::is_same_v<Pin_, PB6>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART1>();
   }

   else if constexpr (mode == PinMode::USART1_RX) {
      static_assert (
         std::is_same_v<Pin_, PA10> or std::is_same_v<Pin_, PB7>,
         "USART1_RX возможно только с PA10 и PB7"
      );
      make_reference<Periph::RCC>()
         .template clock_enable<Periph::AFIO>();
      set<Pin_::n>(Mode::Out2MHz_push_pull_alt);
      if (std::is_same_v<Pin_, PB7>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART1>();
   }

   else if constexpr (mode == PinMode::USART2_TX) {
      static_assert (
         std::is_same_v<Pin_, PA2> or std::is_same_v<Pin_, PD5>,
         "USART2_TX возможно только с PA2 или PD5"
      );
      make_reference<Periph::RCC>()
         .template clock_enable<Periph::AFIO>();
      set<Pin_::n>(Mode::Out2MHz_push_pull_alt);
      if (std::is_same_v<Pin_, PD5>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART2>();
   }

   else if constexpr (mode == PinMode::USART2_RX) {
      static_assert (
         std::is_same_v<Pin_, PA3> or std::is_same_v<Pin_, PD6>,
         "USART2_RX возможно только с PA3 или PD6"
      );
      make_reference<Periph::RCC>()
         .template clock_enable<Periph::AFIO>();
      set<Pin_::n>(Mode::Out2MHz_push_pull_alt);
      if (std::is_same_v<Pin_, PD6>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART2>();
   }

   else if constexpr (mode == PinMode::USART3_TX) {
      static_assert (
         std::is_same_v<Pin_, PB10> or std::is_same_v<Pin_, PC10> or std::is_same_v<Pin_, PD8>,
         "USART3_TX возможно только с PB10, PC10 или PD8"
      );
      make_reference<Periph::RCC>()
         .template clock_enable<Periph::AFIO>();
      set<Pin_::n>(Mode::Out2MHz_push_pull_alt);
      if (std::is_same_v<Pin_, PC10>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART3, Remap::Partial_1>();
      else if (std::is_same_v<Pin_, PD8>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART3, Remap::Full>();
   }

   else if constexpr (mode == PinMode::USART3_RX) {
      static_assert (
         std::is_same_v<Pin_, PB11> or std::is_same_v<Pin_, PC11> or std::is_same_v<Pin_, PD9>,
         "USART3_RX возможно только с PB11, PC11 или PD9"
      );
      make_reference<Periph::RCC>()
         .template clock_enable<Periph::AFIO>();
      set<Pin_::n>(Mode::Out2MHz_push_pull_alt);
      if (std::is_same_v<Pin_, PC11>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART3, Remap::Partial_1>();
      else if (std::is_same_v<Pin_, PD9>)
         make_reference<Periph::AFIO>()
            .template remap<Periph::USART3, Remap::Full>();
   }

}

} //namespace mcu
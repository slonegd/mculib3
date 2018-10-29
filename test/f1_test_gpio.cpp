#define STM32F103xB
#define F_CPU   72000000UL
#include <iostream>
#include <type_traits>
#include "rcc.h"
#include "f1_afio.h"


struct MockRCC {
   bool good {false};
   template <mcu::Periph p> void clock_enable(){good = true;}
}mockRcc;
namespace mcu {
template <Periph p> std::enable_if_t<p == Periph::TEST_RCC, MockRCC&> make_reference() {return mockRcc;}
}

struct MockAFIO {
   bool clock {false};
   bool remap_ {false};
   template <mcu::Periph p, mcu::Periph v> void clock_enable(){clock = true;}
   template <mcu::Periph p> void remap(){remap_ = true;}
   template <mcu::Periph p, mcu::AFIO::Remap> void remap(){remap_ = true;}
}mockAFIO;
namespace mcu {
template <Periph p> std::enable_if_t<p == Periph::TEST_AFIO, MockAFIO&> make_reference() {return mockAFIO;}
}

#include "gpio.h"

mcu::GPIO gpio;
auto& CMSIS = *reinterpret_cast<mcu::GPIO::CMSIS_type*>(&gpio);

bool make()
{
   bool good {true};
   auto& gpioa {mcu::make_reference<mcu::Periph::GPIOA>()};
   good &= reinterpret_cast<size_t>(&gpioa) == GPIOA_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(gpioa)>, mcu::GPIO>;

   auto& gpiob {mcu::make_reference<mcu::Periph::GPIOB>()};
   good &= reinterpret_cast<size_t>(&gpiob) == GPIOB_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(gpiob)>, mcu::GPIO>;

   auto& gpioc {mcu::make_reference<mcu::Periph::GPIOC>()};
   good &= reinterpret_cast<size_t>(&gpioc) == GPIOC_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(gpioc)>, mcu::GPIO>;

   auto& gpiod {mcu::make_reference<mcu::Periph::GPIOD>()};
   good &= reinterpret_cast<size_t>(&gpiod) == GPIOD_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(gpiod)>, mcu::GPIO>;

   auto& gpioe {mcu::make_reference<mcu::Periph::GPIOE>()};
   good &= reinterpret_cast<size_t>(&gpioe) == GPIOE_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(gpioe)>, mcu::GPIO>;

   return good;
}

bool clock_enable()
{
   bool good {true};
   gpio.clock_enable<mcu::Periph::GPIOA, mcu::Periph::TEST_RCC>();
   good &= mockRcc.good;
   return good;
}

bool set()
{
   CMSIS.BSRR = 0;
   bool good {true};
   gpio.set(0);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS0_Msk);
   CMSIS.BSRR = 0;
   gpio.set(1);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS1_Msk);
   CMSIS.BSRR = 0;
   gpio.set(2);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS2_Msk);
   CMSIS.BSRR = 0;
   gpio.set(3);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS3_Msk);
   CMSIS.BSRR = 0;
   gpio.set(4);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS4_Msk);
   CMSIS.BSRR = 0;
   gpio.set(5);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS5_Msk);
   CMSIS.BSRR = 0;
   gpio.set(6);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS6_Msk);
   CMSIS.BSRR = 0;
   gpio.set(7);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS7_Msk);
   CMSIS.BSRR = 0;
   gpio.set(8);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS8_Msk);
   CMSIS.BSRR = 0;
   gpio.set(9);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS9_Msk);
   CMSIS.BSRR = 0;
   gpio.set(10);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS10_Msk);
   CMSIS.BSRR = 0;
   gpio.set(11);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS11_Msk);
   CMSIS.BSRR = 0;
   gpio.set(12);
   int x = GPIO_BSRR_BS1_Msk;
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS12_Msk);
   CMSIS.BSRR = 0;
   gpio.set(13);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS13_Msk);
   CMSIS.BSRR = 0;
   gpio.set(14);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS14_Msk);
   CMSIS.BSRR = 0;
   gpio.set(15);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS15_Msk);

   return good;
}

bool clear()
{
   CMSIS.BSRR = 0;
   bool good {true};
   gpio.clear(0);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR0_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(1);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR1_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(2);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR2_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(3);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR3_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(4);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR4_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(5);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR5_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(6);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR6_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(7);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR7_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(8);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR8_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(9);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR9_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(10);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR10_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(11);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR11_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(12);
   int x = GPIO_BSRR_BS1_Msk;
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR12_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(13);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR13_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(14);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR14_Msk);
   CMSIS.BSRR = 0;
   gpio.clear(15);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR15_Msk);

   return good;
}

bool is_set()
{
   bool good {true};
   CMSIS.IDR |= GPIO_IDR_IDR15_Msk;
   good &= gpio.is_set(15);
   CMSIS.IDR |= GPIO_IDR_IDR11_Msk;
   good &= gpio.is_set(11);
   CMSIS.IDR |= GPIO_IDR_IDR7_Msk;
   good &= gpio.is_set(7);
   CMSIS.IDR |= GPIO_IDR_IDR0_Msk;
   good &= gpio.is_set(0);

   return good;
}

bool toggle()
{
   bool good {true};
   CMSIS.IDR = 0;
   gpio.toggle(0);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS0_Msk);
   gpio.toggle(12);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BS12_Msk);
   CMSIS.IDR |= GPIO_IDR_IDR10_Msk;
   gpio.toggle(10);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR10_Msk);
   CMSIS.IDR |= GPIO_IDR_IDR7_Msk;
   gpio.toggle(7);
   good &= bool (CMSIS.BSRR & GPIO_BSRR_BR7_Msk);

   return good;
}

bool set_mode()
{
   bool good {true};
   gpio.set<0> (mcu::GPIO::Mode::In_analog);
   good &= not (CMSIS.CRL & GPIO_CRL_MODE0_0)
       and not (CMSIS.CRL & GPIO_CRL_MODE0_1)
       and not (CMSIS.CRL & GPIO_CRL_CNF0_0)
       and not (CMSIS.CRL & GPIO_CRL_CNF0_1);

   gpio.set<1> (mcu::GPIO::Mode::In_floating);
   good &= not (CMSIS.CRL & GPIO_CRL_MODE1_0)
       and not (CMSIS.CRL & GPIO_CRL_MODE1_1)
       and bool(CMSIS.CRL & GPIO_CRL_CNF1_0)
       and not (CMSIS.CRL & GPIO_CRL_CNF1_1);

   gpio.set<2> (mcu::GPIO::Mode::In_pull);
   good &= not (CMSIS.CRL & GPIO_CRL_MODE2_0)
       and not (CMSIS.CRL & GPIO_CRL_MODE2_1)
       and not (CMSIS.CRL & GPIO_CRL_CNF2_0)
       and bool(CMSIS.CRL & GPIO_CRL_CNF2_1);

   gpio.set<3> (mcu::GPIO::Mode::Out10MHz_push_pull);
   good &= bool(CMSIS.CRL & GPIO_CRL_MODE3_0)
       and not (CMSIS.CRL & GPIO_CRL_MODE3_1)
       and not (CMSIS.CRL & GPIO_CRL_CNF3_0)
       and not (CMSIS.CRL & GPIO_CRL_CNF3_1);

   gpio.set<4> (mcu::GPIO::Mode::Out10MHz_open_drain);
   good &= bool(CMSIS.CRL & GPIO_CRL_MODE4_0)
       and not (CMSIS.CRL & GPIO_CRL_MODE4_1)
       and bool(CMSIS.CRL & GPIO_CRL_CNF4_0)
       and not (CMSIS.CRL & GPIO_CRL_CNF4_1);

   gpio.set<5> (mcu::GPIO::Mode::Out10MHz_push_pull_alt);
   good &= bool(CMSIS.CRL & GPIO_CRL_MODE5_0)
       and not (CMSIS.CRL & GPIO_CRL_MODE5_1)
       and not (CMSIS.CRL & GPIO_CRL_CNF5_0)
       and bool(CMSIS.CRL & GPIO_CRL_CNF5_1);

   gpio.set<6> (mcu::GPIO::Mode::Out10MHz_open_drain_alt);
   good &= bool(CMSIS.CRL & GPIO_CRL_MODE6_0)
       and not (CMSIS.CRL & GPIO_CRL_MODE6_1)
       and bool(CMSIS.CRL & GPIO_CRL_CNF6_0)
       and bool(CMSIS.CRL & GPIO_CRL_CNF6_1);

   gpio.set<7> (mcu::GPIO::Mode::Out2MHz_push_pull);
   good &= not (CMSIS.CRL & GPIO_CRL_MODE7_0)
       and bool(CMSIS.CRL & GPIO_CRL_MODE7_1)
       and not (CMSIS.CRL & GPIO_CRL_CNF7_0)
       and not (CMSIS.CRL & GPIO_CRL_CNF7_1);

   gpio.set<8> (mcu::GPIO::Mode::Out2MHz_open_drain);
   good &= not (CMSIS.CRH & GPIO_CRH_MODE8_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE8_1)
       and bool(CMSIS.CRH & GPIO_CRH_CNF8_0)
       and not (CMSIS.CRH & GPIO_CRH_CNF8_1);

   gpio.set<9> (mcu::GPIO::Mode::Out2MHz_push_pull_alt);
   good &= not (CMSIS.CRH & GPIO_CRH_MODE9_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE9_1)
       and not (CMSIS.CRH & GPIO_CRH_CNF9_0)
       and bool(CMSIS.CRH & GPIO_CRH_CNF9_1);

   gpio.set<10> (mcu::GPIO::Mode::Out2MHz_open_drain_alt);
   good &= not (CMSIS.CRH & GPIO_CRH_MODE10_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE10_1)
       and bool(CMSIS.CRH & GPIO_CRH_CNF10_0)
       and bool(CMSIS.CRH & GPIO_CRH_CNF10_1);

   gpio.set<11> (mcu::GPIO::Mode::Out50MHz_push_pull);
   good &= bool(CMSIS.CRH & GPIO_CRH_MODE11_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE11_1)
       and not (CMSIS.CRH & GPIO_CRH_CNF11_0)
       and not (CMSIS.CRH & GPIO_CRH_CNF11_1);

   gpio.set<11> (mcu::GPIO::Mode::Out50MHz_push_pull_alt);
   good &= bool(CMSIS.CRH & GPIO_CRH_MODE11_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE11_1)
       and not(CMSIS.CRH & GPIO_CRH_CNF11_0)
       and bool(CMSIS.CRH & GPIO_CRH_CNF11_1);

   gpio.set<12> (mcu::GPIO::Mode::Out50MHz_open_drain);
   good &= bool(CMSIS.CRH & GPIO_CRH_MODE12_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE12_1)
       and bool(CMSIS.CRH & GPIO_CRH_CNF12_0)
       and not (CMSIS.CRH & GPIO_CRH_CNF12_1);

   gpio.set<13> (mcu::GPIO::Mode::Out50MHz_push_pull_alt);
   good &= bool(CMSIS.CRH & GPIO_CRH_MODE13_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE13_1)
       and not (CMSIS.CRH & GPIO_CRH_CNF13_0)
       and bool(CMSIS.CRH & GPIO_CRH_CNF13_1);

   gpio.set<14> (mcu::GPIO::Mode::Out50MHz_open_drain_alt);
   good &= bool(CMSIS.CRH & GPIO_CRH_MODE14_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE14_1)
       and bool(CMSIS.CRH & GPIO_CRH_CNF14_0)
       and bool(CMSIS.CRH & GPIO_CRH_CNF14_1);

   gpio.set<15> (mcu::GPIO::Mode::Out50MHz_push_pull);
   good &= bool(CMSIS.CRH & GPIO_CRH_MODE15_0)
       and bool(CMSIS.CRH & GPIO_CRH_MODE15_1)
       and not (CMSIS.CRH & GPIO_CRH_CNF15_0)
       and not (CMSIS.CRH & GPIO_CRH_CNF15_1);

   return good;
}

bool init()
{
   bool good {true};
   CMSIS.CRH = 0;
   gpio.init<mcu::PA12, mcu::PinMode::Input, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE12_0)
       and not  (CMSIS.CRH & GPIO_CRH_MODE12_1)
       and bool (CMSIS.CRH & GPIO_CRH_CNF12_0)
       and not  (CMSIS.CRH & GPIO_CRH_CNF12_1);

   CMSIS.CRL = 0;
   gpio.init<mcu::PC2, mcu::PinMode::Output, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= not  (CMSIS.CRL & GPIO_CRL_MODE2_0)
       and bool (CMSIS.CRL & GPIO_CRL_MODE2_1)
       and not  (CMSIS.CRL & GPIO_CRL_CNF2_0)
       and not  (CMSIS.CRL & GPIO_CRL_CNF2_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PA9, mcu::PinMode::USART1_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockRcc.good;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE9_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE9_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF9_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF9_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PB6, mcu::PinMode::USART1_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRL & GPIO_CRL_MODE6_0)
       and bool (CMSIS.CRL & GPIO_CRL_MODE6_1)
       and not  (CMSIS.CRL & GPIO_CRL_CNF6_0)
       and bool (CMSIS.CRL & GPIO_CRL_CNF6_1);

   // gpio.init<mcu::PB2, mcu::PinMode::USART1_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   // compilation error:static assertion failed: USART1_TX возможно только с PA9 или PB6

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PA10, mcu::PinMode::USART1_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE10_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE10_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF10_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF10_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PB7, mcu::PinMode::USART1_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRL & GPIO_CRL_MODE7_0)
       and bool (CMSIS.CRL & GPIO_CRL_MODE7_1)
       and not  (CMSIS.CRL & GPIO_CRL_CNF7_0)
       and bool (CMSIS.CRL & GPIO_CRL_CNF7_1);
   
   // gpio.init<mcu::PB2, mcu::PinMode::USART1_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   // compilation error:static assertion failed: USART1_RX возможно только с PA10 и PB7

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PA2, mcu::PinMode::USART2_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= not  (CMSIS.CRL & GPIO_CRL_MODE2_0)
       and bool (CMSIS.CRL & GPIO_CRL_MODE2_1)
       and not  (CMSIS.CRL & GPIO_CRL_CNF2_0)
       and bool (CMSIS.CRL & GPIO_CRL_CNF2_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PD5, mcu::PinMode::USART2_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRL & GPIO_CRL_MODE5_0)
       and bool (CMSIS.CRL & GPIO_CRL_MODE5_1)
       and not  (CMSIS.CRL & GPIO_CRL_CNF5_0)
       and bool (CMSIS.CRL & GPIO_CRL_CNF5_1);

   // gpio.init<mcu::PB2, mcu::PinMode::USART2_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   // compilation error:static assertion failed: USART1_RX возможно только с PA2 или PD5

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PA3, mcu::PinMode::USART2_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= not  (CMSIS.CRL & GPIO_CRL_MODE3_0)
       and bool (CMSIS.CRL & GPIO_CRL_MODE3_1)
       and not  (CMSIS.CRL & GPIO_CRL_CNF3_0)
       and bool (CMSIS.CRL & GPIO_CRL_CNF3_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PD6, mcu::PinMode::USART2_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRL & GPIO_CRL_MODE6_0)
       and bool (CMSIS.CRL & GPIO_CRL_MODE6_1)
       and not  (CMSIS.CRL & GPIO_CRL_CNF6_0)
       and bool (CMSIS.CRL & GPIO_CRL_CNF6_1);

   // gpio.init<mcu::PC2, mcu::PinMode::USART2_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   // compilation error:static assertion failed: USART1_RX возможно только с PA3 иди PD6

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PB10, mcu::PinMode::USART3_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE10_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE10_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF10_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF10_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PC10, mcu::PinMode::USART3_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE10_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE10_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF10_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF10_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PD8, mcu::PinMode::USART3_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE8_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE8_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF8_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF8_1);

   // gpio.init<mcu::PC2, mcu::PinMode::USART3_TX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   // compilation error:static assertion failed: USART1_RX возможно только с PB10, PC10 или PD8

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PB11, mcu::PinMode::USART3_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE11_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE11_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF11_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF11_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PC11, mcu::PinMode::USART3_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE11_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE11_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF11_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF11_1);

   CMSIS.CRL = CMSIS.CRH = 0;
   gpio.init<mcu::PD9, mcu::PinMode::USART3_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   good &= mockAFIO.clock;
   good &= mockAFIO.remap_;
   good &= not  (CMSIS.CRH & GPIO_CRH_MODE9_0)
       and bool (CMSIS.CRH & GPIO_CRH_MODE9_1)
       and not  (CMSIS.CRH & GPIO_CRH_CNF9_0)
       and bool (CMSIS.CRH & GPIO_CRH_CNF9_1);

   // gpio.init<mcu::PA10, mcu::PinMode::USART3_RX, mcu::Periph::TEST_RCC, mcu::Periph::TEST_AFIO>();
   // compilation error:static assertion failed: USART1_RX возможно только с PB11, PC11 или PD9


   return good;
}



int main()
{
   std::cout << '\n'
             << "Тесты класса GPIO для STM32F1:" << std::endl;

   auto test = [](auto s, auto f){
      std::cout << s << (f() ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   };

   test ("RCC::make            ", make);
   test ("RCC::clock_enable    ", clock_enable);
   test ("RCC::set             ", set);
   test ("RCC::clear           ", clear);
   test ("RCC::is_set          ", is_set);
   test ("RCC::toggle          ", toggle);
   test ("RCC::set_mode        ", set_mode);
   test ("RCC::init            ", init);
}
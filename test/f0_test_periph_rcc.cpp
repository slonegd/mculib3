#define BOOST_TEST_MODULE f0_test_periph_rcc
#include <boost/test/unit_test.hpp>

#define F_CPU   48000000UL
#define STM32F030x6

#include "periph_rcc.h"
#include "timeout.h"
#include <iostream>
#include <type_traits>
#include <thread>

BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::RCC rcc;
auto& CMSIS = *reinterpret_cast<mcu::RCC::CMSIS_type*>(&rcc);

BOOST_AUTO_TEST_CASE (make_reference)
{
   auto& rcc {mcu::make_reference<mcu::Periph::RCC>()};
   auto address = reinterpret_cast<size_t>(&rcc);
   auto same = std::is_same_v<std::remove_reference_t<decltype(rcc)>, mcu::RCC>;
   BOOST_CHECK_EQUAL (address, RCC_BASE);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (set_AHBprescaler)
{
   // TODO wrong result, dont check 0, only 1
   CMSIS.CFGR = 0;
   uint32_t result {0};
   rcc.set (mcu::RCC::AHBprescaler::AHBdiv512);
   result = RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv256);
   result = RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv128);
   result = RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv64);
   result = RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv16);
   result = RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv8);
   result = RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv4);
   result = RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv2);
   result = RCC_CFGR_HPRE_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::AHBprescaler::AHBnotdiv);
   result = 0;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);
}

BOOST_AUTO_TEST_CASE (set_APBprescaler)
{
   CMSIS.CFGR = 0;
   uint32_t result {0};
   rcc.set (mcu::RCC::APBprescaler::APBdiv16);
   result = RCC_CFGR_PPRE_0 | RCC_CFGR_PPRE_1 | RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBdiv8);
   result = RCC_CFGR_PPRE_1 | RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBdiv4);
   result = RCC_CFGR_PPRE_0 | RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBdiv2);
   result = RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBnotdiv);
   result = 0;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);
}

BOOST_AUTO_TEST_CASE (set_SystemClock)
{
   CMSIS.CFGR = 0;
   uint32_t result {0};
   rcc.set (mcu::RCC::SystemClock::CS_PLL);
   result = RCC_CFGR_SW_1;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::SystemClock::CS_HSE);
   result = RCC_CFGR_SW_0;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::SystemClock::CS_HSI);
   result = 0;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);
}

BOOST_AUTO_TEST_CASE (set_PLLsource)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::PLLsource::HSE);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PLLSRC_Msk);

   rcc.set (mcu::RCC::PLLsource::HSIdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, 0);
}

BOOST_AUTO_TEST_CASE (set_PLLmultiplier)
{
   CMSIS.CFGR = 0;
   uint32_t result {0};
   rcc.set (mcu::RCC::PLLmultiplier::_16);
   result = RCC_CFGR_PLLMUL_1 | RCC_CFGR_PLLMUL_2 | RCC_CFGR_PLLMUL_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_3), true);

   rcc.set (mcu::RCC::PLLmultiplier::_15);
   result = RCC_CFGR_PLLMUL_0 | RCC_CFGR_PLLMUL_2 | RCC_CFGR_PLLMUL_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::PLLmultiplier::_10);
   result = RCC_CFGR_PLLMUL_3;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::PLLmultiplier::_9);
   result = RCC_CFGR_PLLMUL_0 | RCC_CFGR_PLLMUL_1 | RCC_CFGR_PLLMUL_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::PLLmultiplier::_2);
   result = 0;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);
}

BOOST_AUTO_TEST_CASE (on_PLL)
{
   CMSIS.CR = 0;
   rcc.on_PLL();
   BOOST_CHECK_EQUAL (CMSIS.CR, RCC_CR_PLLON_Msk);
}

BOOST_AUTO_TEST_CASE (wait_PLL_ready)
{
   CMSIS.CR = 0;

   volatile bool work {true};
   auto worker = [&]() { rcc.wait_PLL_ready(); work = false; };
   std::thread {worker}.detach();

   std::this_thread::sleep_for(std::chrono::milliseconds(10));

   BOOST_CHECK_EQUAL (work, true);

   CMSIS.CR |= RCC_CR_PLLRDY_Msk;
   Timeout timeout {100};
   while (work) {
      if (timeout) break;
   }

   BOOST_CHECK_EQUAL (bool(timeout), false);
   BOOST_CHECK_EQUAL (work, false);
}

BOOST_AUTO_TEST_CASE (clock_enable)
{
   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOA>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOAEN_Msk);

   rcc.clock_enable<mcu::Periph::GPIOB>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOBEN_Msk | RCC_AHBENR_GPIOAEN_Msk);

   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOC>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOCEN_Msk);

   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOD>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIODEN_Msk);

   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOF>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOFEN_Msk);
}

BOOST_AUTO_TEST_SUITE_END()

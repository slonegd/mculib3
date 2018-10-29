#define BOOST_TEST_MODULE f0_rcc
#include <boost/test/unit_test.hpp>

#define STM32F030x6

#include "rcc.h"
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
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::AHBprescaler::AHBdiv512);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv256);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv128);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv64);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv16);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv8);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv4);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv2);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_2), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), true);

   rcc.set (mcu::RCC::AHBprescaler::AHBnotdiv);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_HPRE_3), false);
}

BOOST_AUTO_TEST_CASE (set_APBprescaler)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::APBprescaler::APBdiv16);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_2), true);

   rcc.set (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_2), true);

   rcc.set (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_2), true);

   rcc.set (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_2), true);

   rcc.set (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PPRE_2), false);
}

BOOST_AUTO_TEST_CASE (set_SystemClock)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::SystemClock::CS_PLL);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_SW_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_SW_1), true);

   rcc.set (mcu::RCC::SystemClock::CS_HSE);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_SW_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_SW_1), false);

   rcc.set (mcu::RCC::SystemClock::CS_HSI);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_SW_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_SW_1), false);
}

BOOST_AUTO_TEST_CASE (set_PLLsource)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::PLLsource::HSE);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLSRC_Msk), true);

   rcc.set (mcu::RCC::PLLsource::HSIdiv2);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLSRC_Msk), false);
}

BOOST_AUTO_TEST_CASE (set_PLLmultiplier)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::PLLmultiplier::_16);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_3), true);

   rcc.set (mcu::RCC::PLLmultiplier::_15);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_3), true);

   rcc.set (mcu::RCC::PLLmultiplier::_10);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_2), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_3), true);

   rcc.set (mcu::RCC::PLLmultiplier::_9);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_0), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_1), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_2), true);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_3), false);

   rcc.set (mcu::RCC::PLLmultiplier::_2);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_0), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_1), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_2), false);
   BOOST_CHECK_EQUAL (bool(CMSIS.CFGR & RCC_CFGR_PLLMUL_3), false);
}

BOOST_AUTO_TEST_CASE (on_PLL)
{
   CMSIS.CR = 0;
   rcc.on_PLL();
   BOOST_CHECK_EQUAL (bool(CMSIS.CR & RCC_CR_PLLON_Msk), true);
}

BOOST_AUTO_TEST_CASE (wait_PLL_ready)
{
   // TODO: timeout when failed
   CMSIS.CR = 0;
   bool good {true};
   bool work {true};
   auto worker = [&]() { rcc.wait_PLL_ready(); work = false; };
   std::thread {worker}.detach();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   good &= work;
   CMSIS.CR |= RCC_CR_PLLRDY_Msk;
   while (work) {}
}

BOOST_AUTO_TEST_CASE (clock_enable)
{
   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOA>();
   BOOST_CHECK_EQUAL (bool(CMSIS.AHBENR & RCC_AHBENR_GPIOAEN_Msk), true);

   rcc.clock_enable<mcu::Periph::GPIOB>();
   BOOST_CHECK_EQUAL (bool(CMSIS.AHBENR & RCC_AHBENR_GPIOBEN_Msk), true);

   rcc.clock_enable<mcu::Periph::GPIOC>();
   BOOST_CHECK_EQUAL (bool(CMSIS.AHBENR & RCC_AHBENR_GPIOCEN_Msk), true);

   rcc.clock_enable<mcu::Periph::GPIOD>();
   BOOST_CHECK_EQUAL (bool(CMSIS.AHBENR & RCC_AHBENR_GPIODEN_Msk), true);

   rcc.clock_enable<mcu::Periph::GPIOF>();
   BOOST_CHECK_EQUAL (bool(CMSIS.AHBENR & RCC_AHBENR_GPIOFEN_Msk), true);
}

BOOST_AUTO_TEST_SUITE_END()

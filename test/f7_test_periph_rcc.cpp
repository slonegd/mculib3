#define BOOST_TEST_MODULE f7_test_periph_rcc
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx

#include "periph_rcc.h"
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"


BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::RCC rcc {};
auto& CMSIS = rcc.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(CMSIS)>, RCC_TypeDef>;
   auto address_rcc = reinterpret_cast<size_t>(&rcc);
   auto address_CMSIS = reinterpret_cast<size_t>(&CMSIS);
   BOOST_CHECK_EQUAL (address_rcc, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (set_AHBprescaler)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::AHBprescaler::AHBdiv512);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV512);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv256);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV256);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv128);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV128);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv64);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV64);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv16);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV16);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv8);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV8);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv4);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV4);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV2);

   rcc.set (mcu::RCC::AHBprescaler::AHBnotdiv);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV1);
}

BOOST_AUTO_TEST_CASE (set_APB1prescaler)
{
   CMSIS.CFGR = 0;
   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv16);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV16);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV8);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV4);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV1);
}

BOOST_AUTO_TEST_CASE (set_APB2prescaler)
{
   CMSIS.CFGR = 0;
   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv16);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV16);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV8);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV4);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV2);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV1);
}

BOOST_AUTO_TEST_CASE (set_SystemClock)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::SystemClock::CS_PLL);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_SW_PLL);

   rcc.set (mcu::RCC::SystemClock::CS_HSE);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_SW_HSE);

   rcc.set (mcu::RCC::SystemClock::CS_HSI);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_SW_HSI);
}

BOOST_AUTO_TEST_CASE (set_PLLPdiv)
{
   CMSIS.PLLCFGR = 0;
   rcc.set (mcu::RCC::PLLPdiv::PLLdiv8);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1);

   rcc.set (mcu::RCC::PLLPdiv::PLLdiv6);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLP_1);

   rcc.set (mcu::RCC::PLLPdiv::PLLdiv4);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLP_0);

   rcc.set (mcu::RCC::PLLPdiv::PLLdiv2);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 0);
}

BOOST_AUTO_TEST_CASE (set_PLLsource)
{
   CMSIS.PLLCFGR = 0;
   rcc.set (mcu::RCC::PLLsource::HSE);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);

   rcc.set (mcu::RCC::PLLsource::HSIdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_PLLCFGR_PLLSRC_HSI);
}

BOOST_AUTO_TEST_CASE (get_APB_clock)
{
   rcc.set_APB1 (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (rcc.get_APB1_clock(), F_CPU);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (rcc.get_APB2_clock(), F_CPU / 2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (rcc.get_APB1_clock(), F_CPU / 4);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (rcc.get_APB2_clock(), F_CPU / 8);
}

BOOST_AUTO_TEST_CASE (set_PLLM)
{
   CMSIS.PLLCFGR = 0;
   // rcc.set_PLLM<0>();  // compilation error
   // rcc.set_PLLM<1>();  // compilation error
   // rcc.set_PLLM<64>(); // compilation error
   rcc.set_PLLM<63>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_0 
      | RCC_PLLCFGR_PLLM_1
      | RCC_PLLCFGR_PLLM_2
      | RCC_PLLCFGR_PLLM_3
      | RCC_PLLCFGR_PLLM_4
      | RCC_PLLCFGR_PLLM_5
   );

   rcc.set_PLLM<15>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_0 
      | RCC_PLLCFGR_PLLM_1
      | RCC_PLLCFGR_PLLM_2
      | RCC_PLLCFGR_PLLM_3
   );

   rcc.set_PLLM<10>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_1
      | RCC_PLLCFGR_PLLM_3
   );

   rcc.set_PLLM<9>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_0
      | RCC_PLLCFGR_PLLM_3
   );

   rcc.set_PLLM<2>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLM_1);
}

BOOST_AUTO_TEST_CASE (on_HSE)
{
   CMSIS.CR = 0;
   rcc.on_HSE();
   BOOST_CHECK_EQUAL (CMSIS.CR, RCC_CR_HSEON_Msk);
}

BOOST_AUTO_TEST_CASE (wait_HSE_ready)
{
   CMSIS.CR = 0;

   volatile bool work {true};
   auto worker = [&]() { rcc.wait_HSE_ready(); work = false; };
   std::thread {worker}.detach();

   std::this_thread::sleep_for(std::chrono::milliseconds(10));

   BOOST_CHECK_EQUAL (work, true);

   CMSIS.CR |= RCC_CR_HSERDY_Msk;
   Timeout timeout {100};
   while (work) {
      if (timeout) break;
   }

   BOOST_CHECK_EQUAL (bool(timeout), false);
   BOOST_CHECK_EQUAL (work, false);
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
   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOA>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOAEN_Msk);

   rcc.clock_enable<mcu::Periph::GPIOB>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOBEN_Msk | RCC_AHB1ENR_GPIOAEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOC>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOCEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOD>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIODEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOE>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOEEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOF>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOFEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOG>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOGEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOH>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOHEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOI>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOIEN_Msk);



   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::USART1>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_USART1EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART2>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_USART2EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART3>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_USART3EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART4>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART4EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART5>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART5EN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::USART6>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_USART6EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART7>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART7EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART8>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART8EN_Msk);
}

BOOST_AUTO_TEST_SUITE_END()

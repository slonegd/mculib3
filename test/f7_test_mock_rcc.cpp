#define BOOST_TEST_MODULE f7_test_rcc
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx
#define TEST

#include "mock_rcc.h"
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"


BOOST_AUTO_TEST_SUITE (test_suite_main)

auto& rcc = mcu::make_reference<mcu::Periph::RCC>();
auto& CMSIS = rcc.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   std::stringstream process;
   rcc.set_stream (process);
   rcc.set      (mcu::RCC::AHBprescaler::AHBnotdiv)
      .set_APB1 (mcu::RCC::APBprescaler::APBdiv2)
      .set_APB2 (mcu::RCC::APBprescaler::APBdiv4)
      .set      (mcu::RCC::SystemClock::CS_PLL);
   BOOST_CHECK_EQUAL (process.str(),
      "установка делителя шины AHB 1\n"
      "установка делителя шины APB1 2\n" 
      "установка делителя шины APB2 4\n" 
      "установка источника тактирования системной шины от PLL\n"
   );
}

BOOST_AUTO_TEST_SUITE_END()

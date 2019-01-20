#define BOOST_TEST_MODULE f7_init_clock
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx
#define TEST

#include "mock_rcc.h"
#include "mock_flash.h"
#include "mock_gpio.h"
#include "init_clock.h"
#include <iostream>
#include <thread>

auto& process = mock::Process::make();


BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (init_clock_8_216)
{
   auto& rcc   = mcu::make_reference<mcu::Periph::RCC>();
   auto& flash = mcu::make_reference<mcu::Periph::FLASH>();

   process.clear();

   auto worker = [&]() { init_clock<8_MHz, F_CPU>(); };
   std::thread {worker}.detach();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));

   BOOST_CHECK_EQUAL (process.str(),
      "установка задержки чтения памяти 7\n"
      "включение тактирования от внешнего источника\n"
      "ожидание готовности тактирования от внешнего источника\n"
      "установка делителя шины AHB 1\n"
      "установка делителя шины APB1 4\n"
      "установка делителя шины APB2 2\n"
      "установка источника тактирования системной шины от PLL\n"
      "установка M 4\n"
      "установка N 216\n"
      "установка делителя шины PLL 2\n"
      "установка источника PLL внешний\n"
      "включение PLL\n"
      "ожидание готовности PLL\n"    
   );
}

BOOST_AUTO_TEST_CASE (init_clock_25_216)
{
   auto& rcc   = mcu::make_reference<mcu::Periph::RCC>();
   auto& flash = mcu::make_reference<mcu::Periph::FLASH>();
   
   process.clear();

   auto worker = [&]() { init_clock<25_MHz, F_CPU>(); };
   std::thread {worker}.detach();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));

   BOOST_CHECK_EQUAL (process.str(),
      "установка задержки чтения памяти 7\n"
      "включение тактирования от внешнего источника\n"
      "ожидание готовности тактирования от внешнего источника\n"
      "установка делителя шины AHB 1\n"
      "установка делителя шины APB1 4\n"
      "установка делителя шины APB2 2\n"
      "установка источника тактирования системной шины от PLL\n"
      "установка M 25\n"
      "установка N 432\n"
      "установка делителя шины PLL 2\n"
      "установка источника PLL внешний\n"
      "включение PLL\n"
      "ожидание готовности PLL\n"    
   );
}

BOOST_AUTO_TEST_CASE (init_clock_216)
{
   auto& rcc   = mcu::make_reference<mcu::Periph::RCC>();
   auto& flash = mcu::make_reference<mcu::Periph::FLASH>();
   
   process.clear();

   auto worker = [&]() { init_clock<F_CPU>(); };
   std::thread {worker}.detach();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));

   BOOST_CHECK_EQUAL (process.str(),
      "установка задержки чтения памяти 7\n"
      "установка делителя шины AHB 1\n"
      "установка делителя шины APB1 4\n"
      "установка делителя шины APB2 2\n"
      "установка источника тактирования системной шины от PLL\n"
      "установка M 8\n"
      "установка N 432\n"
      "установка делителя шины PLL 2\n"
      "установка источника PLL внутренний с делителем 2\n"
      "включение PLL\n"
      "ожидание готовности PLL\n"    
   );
}

BOOST_AUTO_TEST_SUITE_END()

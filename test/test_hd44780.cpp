#define BOOST_TEST_MODULE test_hd44780
#include <boost/test/unit_test.hpp>
#include <thread>

#define TEST
#define F_CPU   72000000UL
#define STM32F103xB

#include "timeout.h"
#include "mock_systick.h"
#include "mock_rcc.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "hd44780.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

using RS  = mcu::PC12;
using RW  = mcu::PD2;
using E   = mcu::PB3;
using DB4 = mcu::PB4;
using DB5 = mcu::PB5;
using DB6 = mcu::PB6;
using DB7 = mcu::PB7;

// void tick (int times)
// {
//    while (times) {
//       SysTick_Handler();
//       times--;
//    }
// }


std::array<char, 80> buffer {'H', 'e', 'l', 'l', 'o'};
auto& e = Pin::make<E>();
auto& rs = Pin::make<RS>();
auto& rw = Pin::make<RW>();


BOOST_AUTO_TEST_CASE(make)
{
   auto& systick = mcu::make_reference<mcu::Periph::SysTick>();
   
   bool work {false};
   std::thread ([&](){
      HD44780::make<RS, RW, E, DB4, DB5, DB6, DB7>(buffer);
   }).detach();
   // Timeout time {20};
 
   auto begin = systick.now();
   // std::thread ([&](){systick.tick();}).detach();

   while (not e) {systick.tick();}
   auto passed_ms = systick.microseconds_from(begin);

   BOOST_CHECK (passed_ms >= 20'000);
   // BOOST_CHECK (time_passed >= 20);
   // BOOST_CHECK_EQUAL (bool(e), false);

   // main_pros.join();
}

BOOST_AUTO_TEST_SUITE_END()
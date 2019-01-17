#define BOOST_TEST_MODULE test_delay
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB
#define TEST


#include "mock_systick.h"
#include "mock_timer.h"
#include "delay.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

Delay delay;

BOOST_AUTO_TEST_CASE(for_ms)
{
   bool done = delay.ms(2);
   BOOST_CHECK_EQUAL (done, false);

   done = delay.ms(10);
   BOOST_CHECK_EQUAL (done, false);
}



BOOST_AUTO_TEST_CASE(for_us)
{
   auto& systick = mcu::make_reference<mcu::Periph::SysTick>();
   uint32_t us = 200;
   uint32_t pause = us * F_CPU/1'000'000;
   uint32_t left = (systick.get_load() - pause - 1);
   
   while (pause--) {
      systick.tick();
      BOOST_CHECK_EQUAL (delay.us(us), true);
   }

   systick.tick();
   BOOST_CHECK_EQUAL (delay.us(us), false);
   BOOST_CHECK_EQUAL (systick.get(), left);
}

BOOST_AUTO_TEST_CASE(for_us_zero_tick)
{
   auto& systick = mcu::make_reference<mcu::Periph::SysTick>();
   uint32_t us = 200;
   uint32_t pause = us * F_CPU/1'000'000;
   uint32_t decrease_ticks = systick.get_load() - pause;

   while (decrease_ticks--) {
      systick.tick();
   }

   while (pause--) {
      systick.tick();
      BOOST_CHECK_EQUAL (delay.us(us), true);
   }

   systick.tick();
   BOOST_CHECK_EQUAL (delay.us(us), false);
   BOOST_CHECK_EQUAL (delay.us(us), true);

}

BOOST_AUTO_TEST_SUITE_END()
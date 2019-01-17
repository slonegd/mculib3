#define BOOST_TEST_MODULE test_timers
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB

#include "mock_systick.h"
#include "timers.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

Timer timer {20};
auto passed_ms = [&](int n){while (n--) SysTick_Handler();}; 

BOOST_AUTO_TEST_CASE(constructor)
{
   BOOST_CHECK_EQUAL (timer.timeSet, 20);
}

BOOST_AUTO_TEST_CASE (event)
{
   auto times {19};
   while(times--) {
      passed_ms(1);
      BOOST_CHECK_EQUAL (timer.event(), false);
   }
   passed_ms(1);
   BOOST_CHECK_EQUAL (timer.event(), true);
   BOOST_CHECK_EQUAL (timer.timePassed(), 0);
}


BOOST_AUTO_TEST_CASE(done)
{
   timer.start (20);
   auto times {19};
   while(times--) {
      passed_ms(1);
      BOOST_CHECK_EQUAL (timer.done(), false);
   }
   passed_ms(1);
   BOOST_CHECK_EQUAL (timer.done(), true);
   BOOST_CHECK_EQUAL (timer.timePassed(), 20);
   timer.stop();
}

BOOST_AUTO_TEST_CASE(pause_start)
{
   timer.start (20);
  
   passed_ms(10);
   BOOST_CHECK_EQUAL (timer.done(), false);
   timer.pause();
   passed_ms(10);
   BOOST_CHECK_EQUAL (timer.done(), false);
   timer.start();
   passed_ms(10);
   BOOST_CHECK_EQUAL (timer.done(), true);
   timer.stop();
}

BOOST_AUTO_TEST_CASE(stop_start)
{
   timer.start (20);
  
   passed_ms(10);
   BOOST_CHECK_EQUAL (timer.done(), false);
   timer.stop();
   passed_ms(10);
   BOOST_CHECK_EQUAL (timer.done(), false);
   timer.start();
   passed_ms(20);
   BOOST_CHECK_EQUAL (timer.done(), true);
   timer.stop();
}

BOOST_AUTO_TEST_CASE(isGreater)
{
   timer.start (20);
  
   passed_ms(10);
   timer.pause();
   BOOST_CHECK_EQUAL (timer.isGreater(15), false);
   BOOST_CHECK_EQUAL (timer.isGreater(5),  true);
   timer.stop();
}

BOOST_AUTO_TEST_CASE(isCount)
{
   timer.start (20);
  
   passed_ms(10);
   timer.pause();
   BOOST_CHECK_EQUAL (timer.isCount(), false);
   timer.start();
   BOOST_CHECK_EQUAL (timer.isCount(),  true);
   timer.stop();
}

BOOST_AUTO_TEST_CASE(passed_left)
{
   timer.start (20);
  
   passed_ms(12);
   timer.pause();
   BOOST_CHECK_EQUAL (timer.timePassed(), 12);
   BOOST_CHECK_EQUAL (timer.timeLeft(),    8);
   timer.stop();
}

int test_number = 5;
void sum () {++test_number;}

BOOST_AUTO_TEST_CASE(event_function)
{
   timer.start (20);

   passed_ms(20);
   timer.event(sum);
   BOOST_CHECK_EQUAL (test_number, 6);
   timer.stop();
}

BOOST_AUTO_TEST_SUITE_END()


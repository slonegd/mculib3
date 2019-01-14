#define BOOST_TEST_MODULE f1_test_periph_usart_2
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB
#define TEST

#include "periph_usart.h"

#include "timeout.h"
#include "literals.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)


mcu::USART usart;
auto& cmsis = usart.like_CMSIS();

BOOST_AUTO_TEST_CASE (clear_interrupt_flags)
{
   struct mock : mcu::USART {
      volatile bool SR_readed {false};
      uint32_t read_SR() override { SR_readed = true; }
      volatile bool DR_readed {false};
      uint32_t read_DR() override { DR_readed = true; }
   } usart;

   auto worker = [&]() { usart.clear_interrupt_flags(); };
   std::thread {worker}.detach();

   Timeout timeout {200_ms};
   while (not usart.SR_readed and not timeout) {}
   BOOST_CHECK_EQUAL(bool(timeout), false);
   BOOST_CHECK_EQUAL(usart.SR_readed, true);
   BOOST_CHECK_EQUAL(usart.DR_readed, false);

   while (not usart.DR_readed and not timeout) {}
   BOOST_CHECK_EQUAL(bool(timeout), false);
   BOOST_CHECK_EQUAL(usart.SR_readed, true);
   BOOST_CHECK_EQUAL(usart.DR_readed, true);
}

BOOST_AUTO_TEST_SUITE_END()
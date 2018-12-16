#define BOOST_TEST_MODULE f7_test_make_reference
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx

#include "rcc.h"
// #include <iostream>
#include <type_traits>
// #include <thread>
// #include "timeout.h"


BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (rcc)
{
   auto& rcc {mcu::make_reference<mcu::Periph::RCC>()};
   auto address = reinterpret_cast<size_t>(&rcc);
   auto same = std::is_same_v<std::remove_reference_t<decltype(rcc)>, mcu::RCC>;
   BOOST_CHECK_EQUAL (address, RCC_BASE);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_SUITE_END()

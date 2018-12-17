#define BOOST_TEST_MODULE f7_test_make_reference
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx

#include "rcc.h"
#include "periph_flash.h"
#include <type_traits>


BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (rcc)
{
   auto& rcc {mcu::make_reference<mcu::Periph::RCC>()};
   auto address = reinterpret_cast<size_t>(&rcc);
   auto size = sizeof(rcc);
   auto same = std::is_same_v<std::remove_reference_t<decltype(rcc)>, mcu::RCC>;
   BOOST_CHECK_EQUAL (address, RCC_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::RCC::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (flash)
{
   auto& flash {mcu::make_reference<mcu::Periph::FLASH>()};
   auto address = reinterpret_cast<size_t>(&flash);
   auto size = sizeof(flash);
   auto same = std::is_same_v<std::remove_reference_t<decltype(flash)>, mcu::FLASH>;
   BOOST_CHECK_EQUAL (address, FLASH_R_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::FLASH::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_SUITE_END()

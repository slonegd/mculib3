#define BOOST_TEST_MODULE f7_test_make_reference
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx

#include "periph_rcc.h"
#include "periph_flash.h"
#include "periph_gpio.h"
#include <type_traits>


BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (RCC)
{
   auto& rcc {mcu::make_reference<mcu::Periph::RCC>()};
   auto address = reinterpret_cast<size_t>(&rcc);
   auto size = sizeof(rcc);
   auto same = std::is_same_v<std::remove_reference_t<decltype(rcc)>, mcu::RCC>;
   BOOST_CHECK_EQUAL (address, RCC_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::RCC::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (FLASH)
{
   auto& flash {mcu::make_reference<mcu::Periph::FLASH>()};
   auto address = reinterpret_cast<size_t>(&flash);
   auto size = sizeof(flash);
   auto same = std::is_same_v<std::remove_reference_t<decltype(flash)>, mcu::FLASH>;
   BOOST_CHECK_EQUAL (address, FLASH_R_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::FLASH::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOA)
{
   auto& PA {mcu::make_reference<mcu::Periph::GPIOA>()};
   auto address = reinterpret_cast<size_t>(&PA);
   auto size = sizeof(PA);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PA)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOA_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOB)
{
   auto& PB {mcu::make_reference<mcu::Periph::GPIOB>()};
   auto address = reinterpret_cast<size_t>(&PB);
   auto size = sizeof(PB);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PB)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOB_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOC)
{
   auto& PC {mcu::make_reference<mcu::Periph::GPIOC>()};
   auto address = reinterpret_cast<size_t>(&PC);
   auto size = sizeof(PC);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PC)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOC_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOD)
{
   auto& PD {mcu::make_reference<mcu::Periph::GPIOD>()};
   auto address = reinterpret_cast<size_t>(&PD);
   auto size = sizeof(PD);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PD)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOD_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOE)
{
   auto& PE {mcu::make_reference<mcu::Periph::GPIOE>()};
   auto address = reinterpret_cast<size_t>(&PE);
   auto size = sizeof(PE);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PE)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOE_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOF)
{
   auto& PF {mcu::make_reference<mcu::Periph::GPIOF>()};
   auto address = reinterpret_cast<size_t>(&PF);
   auto size = sizeof(PF);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PF)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOF_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOG)
{
   auto& PG {mcu::make_reference<mcu::Periph::GPIOG>()};
   auto address = reinterpret_cast<size_t>(&PG);
   auto size = sizeof(PG);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PG)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOG_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOH)
{
   auto& PH {mcu::make_reference<mcu::Periph::GPIOH>()};
   auto address = reinterpret_cast<size_t>(&PH);
   auto size = sizeof(PH);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PH)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOH_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (GPIOI)
{
   auto& PI {mcu::make_reference<mcu::Periph::GPIOI>()};
   auto address = reinterpret_cast<size_t>(&PI);
   auto size = sizeof(PI);
   auto same = std::is_same_v<std::remove_reference_t<decltype(PI)>, mcu::GPIO>;
   BOOST_CHECK_EQUAL (address, GPIOI_BASE);
   BOOST_CHECK_EQUAL (size, sizeof(mcu::GPIO::CMSIS_type));
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_SUITE_END()

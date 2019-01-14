#define BOOST_TEST_MODULE test_hd44780
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB

#include "mock_gpio.h"
#include "hd44780.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

namespace mock {

std::array<char, 10> mock_buffer;

}

BOOST_AUTO_TEST_CASE(make)
{
   
}

BOOST_AUTO_TEST_SUITE_END()
#define BOOST_TEST_MODULE f0_test_pin
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F030x6
#define TEST

#include "mock_rcc.h"
#include "mock_gpio.h"
#include "pin.h"
#include <type_traits>

#include "f0_f4_f7_test_pin.h"
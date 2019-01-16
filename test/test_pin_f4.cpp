#define BOOST_TEST_MODULE f0_test_pin
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx
#define TEST

#include "mock_rcc.h"
#include "mock_gpio.h"
#include "pin.h"
#include <type_traits>

#include "test_pin_f0_f1_f4_f7.h"
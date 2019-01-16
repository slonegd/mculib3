#define BOOST_TEST_MODULE f1_test_pin
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB

#include "mock_rcc.h"
#include "mock_afio.h"
#include "catch_static_assert.h"
#include "mock_gpio.h"
#undef static_assert
#include "pin.h"
#include <type_traits>

#include "test_pin_f0_f1_f4_f7.h"
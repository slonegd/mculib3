#define BOOST_TEST_MODULE f7_test_periph_gpio
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx

#include "periph_gpio.h"
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"

#include "f0_f4_f7_test_periph_gpio.h"

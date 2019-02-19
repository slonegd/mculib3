#define BOOST_TEST_MODULE f0_test_periph_tim
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F030x6

#include "periph_tim.h"
#include <iostream>
#include <type_traits>

#include "test_periph_tim_f0_f4.h"
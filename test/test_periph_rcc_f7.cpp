#define BOOST_TEST_MODULE f7_test_periph_rcc
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx

#include "catch_static_assert.h"
#include "periph_rcc.h"
#undef static_assert
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"

#include "test_periph_rcc_f0_f1_f4_f7.h"
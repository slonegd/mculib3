#define BOOST_TEST_MODULE f1_test_periph_usart
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB

#include <type_traits>
#include "mock_rcc.h"
#include "meta.h"
#include "catch_static_assert.h"
#include "periph_usart.h"
#undef static_assert

#include "f0_f1_f4_test_periph_usart.h"

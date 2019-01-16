#define BOOST_TEST_MODULE f4_test_periph_usart
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx

#include <type_traits>
#include "mock_rcc.h"
#include "meta.h"
#include "catch_static_assert.h"
#include "periph_usart.h"
#undef static_assert

#include "test_periph_usart_f0_f1_f4.h"

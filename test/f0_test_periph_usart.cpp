#define BOOST_TEST_MODULE f0_test_periph_usart
#include <boost/test/unit_test.hpp>

#define F_CPU   48000000UL
#define STM32F030x6

#include <type_traits>
#include "mock_rcc.h"
#include "meta.h"
#include "catch_static_assert.h"
#include "periph_usart.h"
#undef static_assert

// different name in f0 and f1,f4
#define USART_SR_IDLE_Msk USART_ISR_IDLE_Msk
#define USART_SR_TC_Msk   USART_ISR_TC_Msk

#include "f0_f1_f4_test_periph_usart.h"

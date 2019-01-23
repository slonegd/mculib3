#define BOOST_TEST_MODULE f0_test_periph_rcc
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB


#include "catch_static_assert.h"
#include "periph_rcc.h"
#undef static_assert
#include "timeout.h"
#include <iostream>
#include <type_traits>
#include <thread>

// not defined in CMSIS
#define RCC_CFGR_PLLSRC_HSI_DIV2    (0)
#define RCC_CFGR_PLLSRC_HSE_PREDIV  RCC_CFGR_PLLSRC_Msk
#define RCC_CFGR_PLLMUL_0           RCC_CFGR_PLLMULL_0
#define RCC_CFGR_PLLMUL_1           RCC_CFGR_PLLMULL_1
#define RCC_CFGR_PLLMUL_2           RCC_CFGR_PLLMULL_2
#define RCC_CFGR_PLLMUL_3           RCC_CFGR_PLLMULL_3

#include "test_periph_rcc_f0_f1_f4_f7.h"

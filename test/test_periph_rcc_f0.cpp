#define BOOST_TEST_MODULE f0_test_periph_rcc
#include <boost/test/unit_test.hpp>

#define F_CPU   48000000UL
#define STM32F030x6

#include "catch_static_assert.h"
#include "periph_rcc.h"
#undef static_assert
#include "timeout.h"
#include <iostream>
#include <type_traits>
#include <thread>

// dont exist in CMSIS f0, but exist in f1
#define RCC_AHBENR_DMA1EN_Msk RCC_AHBENR_DMAEN_Msk

#include "test_periph_rcc_f0_f1_f4_f7.h"

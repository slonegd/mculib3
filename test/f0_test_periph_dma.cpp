#define BOOST_TEST_MODULE f0_test_periph_dma
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F030x6


#include <iostream>
#include <type_traits>
#include "periph_dma.h"

#include "f0_f1_test_periph_dma.h"
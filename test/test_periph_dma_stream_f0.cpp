#define BOOST_TEST_MODULE f0_test_periph_dma_stream
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F030x6

#include <iostream>
#include <type_traits>
#include "periph_dma_stream.h"

#include "test_periph_dma_stream_f0_f1.h"
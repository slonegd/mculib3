#define BOOST_TEST_MODULE f7_test_periph_dma_stream
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx


#include <iostream>
#include <type_traits>
#include "periph_dma_stream.h"

#include "test_periph_dma_stream_f4_f7.h"
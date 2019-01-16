#define BOOST_TEST_MODULE f4_test_periph_dma_stream
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx


#include <iostream>
#include <type_traits>
#include "periph_dma_stream.h"

#include "test_periph_dma_stream_f4_f7.h"
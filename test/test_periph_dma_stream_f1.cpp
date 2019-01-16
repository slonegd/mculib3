#define BOOST_TEST_MODULE f1_test_periph_dma_stream
#include <boost/test/unit_test.hpp>

#define STM32F103xB
#define TEST
#define F_CPU 72000000UL


#include <iostream>
#include <type_traits>
#include "periph_dma_stream.h"

#include "test_periph_dma_stream_f0_f1.h"
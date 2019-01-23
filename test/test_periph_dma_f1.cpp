#define BOOST_TEST_MODULE f1_test_periph_dma
#include <boost/test/unit_test.hpp>

#define STM32F103xB
#define F_CPU 72000000


#include <iostream>
#include <type_traits>
#include "periph_dma.h"

#include "test_periph_dma_f0_f1.h"
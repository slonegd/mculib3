#define BOOST_TEST_MODULE f1_test_make_reference
#include <boost/test/unit_test.hpp>

#define STM32F103xB
#define F_CPU 72'000'000UL

#include "periph_rcc.h"
#include "periph_flash.h"
#include "periph_gpio.h"
#include "periph_dma.h"
#include "periph_dma_stream.h"
#include <type_traits>

#include "f0_f1_f4_f7_test_make_reference.h"

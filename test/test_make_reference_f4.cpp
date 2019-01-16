#define BOOST_TEST_MODULE f4_test_make_reference
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx

#include "periph_rcc.h"
#include "periph_flash.h"
#include "periph_gpio.h"
#include "periph_dma.h"
#include "periph_dma_stream.h"
#include <type_traits>

#include "test_make_reference_f0_f1_f4_f7.h"

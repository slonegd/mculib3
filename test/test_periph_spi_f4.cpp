#define BOOST_TEST_MODULE f4_test_periph_spi
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F405xx

#include <type_traits>
#include "mock_rcc.h"
#include "periph_spi.h"

#include "test_periph_spi_f4.h"
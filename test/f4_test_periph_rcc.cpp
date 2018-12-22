#define BOOST_TEST_MODULE f7_test_periph_rcc
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx

#include "periph_rcc.h"
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"

#include "f0_f1_f4_f7_test_periph_rcc.h"
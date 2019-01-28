#define BOOST_TEST_MODULE f1_test_periph_flash
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F103xB
#define TEST
#undef USE_MOCKS

#include "periph_flash.h"
#include "timeout.h"
#include "literals.h"
#include <iostream>
#include <type_traits>
#include <thread>

#include "test_periph_flash_f0_f1.h"
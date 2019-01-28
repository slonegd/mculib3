#define BOOST_TEST_MODULE f1_test_flash
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F103xB
#define TEST

#include "mock_flash.h"
#include "mock_timer.h"
#include "flash.h"
#include <type_traits>

#include "test_flash_f0_f1_f4_f7.h"
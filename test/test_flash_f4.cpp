#define BOOST_TEST_MODULE f4_test_flash
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx
#define TEST

#include "mock_flash.h"
#include "mock_timer.h"
#include "flash.h"
#include <type_traits>

#include "test_flash_f0_f4_f7.h"
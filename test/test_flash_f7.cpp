#define BOOST_TEST_MODULE f7_test_flash
#include <boost/test/unit_test.hpp>

#define F_CPU   216'000'000UL
#define STM32F746xx
#define TEST

#include "mock_flash.h"
#include "flash.h"
#include <type_traits>

#include "test_flash_f0_f4_f7.h"
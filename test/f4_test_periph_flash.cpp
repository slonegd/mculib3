#define BOOST_TEST_MODULE f4_test_periph_flash
#include <boost/test/unit_test.hpp>

#define STM32F405xx
#define TEST
#undef USE_MOCKS

#include "periph_flash.h"
#include "timeout.h"
#include "literals.h"
#include <iostream>
#include <type_traits>
#include <thread>

#include "f4_f7_test_periph_flash.h"

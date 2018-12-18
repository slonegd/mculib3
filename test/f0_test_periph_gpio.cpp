#define BOOST_TEST_MODULE f0_test_periph_gpio
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F030x6

#include "periph_gpio.h"
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"

// отсутсвуют в cmsis f0, но есть в f7
#define GPIO_AFRL_AFRL0_0 (1 << GPIO_AFRL_AFRL0_Pos + 0)
#define GPIO_AFRL_AFRL1_0 (1 << GPIO_AFRL_AFRL1_Pos + 0)
#define GPIO_AFRL_AFRL1_1 (1 << GPIO_AFRL_AFRL1_Pos + 1)

#define GPIO_AFRH_AFRH0_0 (1 << GPIO_AFRH_AFRH0_Pos + 0)
#define GPIO_AFRH_AFRH7_0 (1 << GPIO_AFRH_AFRH7_Pos + 0)
#define GPIO_AFRH_AFRH7_1 (1 << GPIO_AFRH_AFRH7_Pos + 1)
#define GPIO_AFRH_AFRH7_2 (1 << GPIO_AFRH_AFRH7_Pos + 2)


#include "f0_f4_f7_test_periph_gpio.h"


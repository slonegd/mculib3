#define BOOST_TEST_MODULE f4_test_periph_gpio
#include <boost/test/unit_test.hpp>

#define F_CPU   168'000'000UL
#define STM32F405xx

#include "periph_gpio.h"
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"

// отсутсвуют в cmsis f4, но есть в f7
#define GPIO_AFRL_AFRL0_Msk (GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL0_1 | GPIO_AFRL_AFRL0_2 | GPIO_AFRL_AFRL0_3)
#define GPIO_AFRL_AFRL7_Msk (GPIO_AFRL_AFRL7_0 | GPIO_AFRL_AFRL7_1 | GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_3)
#define GPIO_AFRH_AFRH0_Msk (GPIO_AFRH_AFRH0_0 | GPIO_AFRH_AFRH0_1 | GPIO_AFRH_AFRH0_2 | GPIO_AFRH_AFRH0_3)
#define GPIO_AFRH_AFRH7_Msk (GPIO_AFRH_AFRH7_0 | GPIO_AFRH_AFRH7_1 | GPIO_AFRH_AFRH7_2 | GPIO_AFRH_AFRH7_3)


#include "f0_f1_f4_f7_test_periph_gpio.h"
